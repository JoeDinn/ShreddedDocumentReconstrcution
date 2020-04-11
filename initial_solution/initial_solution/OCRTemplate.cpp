#include "stdafx.h"
#include "OCRTemplate.h"
#include <deque>
#include <queue>

OCRTemplate::OCRTemplate()
{
}


OCRTemplate::~OCRTemplate()
{
}

long double OCRTemplate::cost(Fragment & left_image, Fragment & right_image)
{
#ifdef DEBUG
	std::cout << left_image.name << " " << right_image.name << std::endl;
#endif

	//Combine images
	int best_height{ best_height_match(left_image,right_image) };
	Fragment combined_image{ combine(left_image,right_image,best_height) };


	//Get all of the canditate images
	std::vector<SearchBox> candidate_characters{ get_candidate_characters(combined_image) };

	long double score{ 0 };
	int number_of_chars{ 0 };
	//For each candidate character
	for (SearchBox character_box : candidate_characters)
	{
		if (character_box.width() > (0.5)*smallest_width and character_box.height() > (0.75)*smallest_height//Check dimensions arent too small for detection
			and character_box.in_left_strip and character_box.in_right_strip)//Check components cross boundary
		{
			//Make image of char
			cv::Mat character_image(combined_image.image,
				cv::Range(character_box.top_most, MIN((character_box.bottom_most + 1), combined_image.image.rows)), // rows
				cv::Range(character_box.left_most, MIN(character_box.right_most + 1, combined_image.image.cols)));// cols																				  //Pad 
			pad(character_image);
#ifdef DEBUG
			std::cout << " left: " << character_box.left_most << " Right: " << character_box.right_most << " top: " << character_box.top_most << " bottom: " << character_box.bottom_most << std::endl;
#endif // DEBUG


#ifdef DEBUG
			cv::rectangle(visualise_image, cv::Point(character_box.left_most - 1, character_box.top_most - 1), cv::Point(character_box.right_most + 1, character_box.bottom_most + 1), CV_RGB(0, 255, 0), 1, 8, 0);
#endif // DEBUG
			//Get score
			score += char_score(character_image);
		}
#ifdef DEBUG
		else
		{
			if (character_box.width() <= (3.0 / 4)*smallest_width or character_box.height() <= (3.0 / 4)*smallest_height)
			{
				cv::rectangle(visualise_image, cv::Point(character_box.left_most - 1, character_box.top_most - 1), cv::Point(character_box.right_most + 1, character_box.bottom_most + 1), CV_RGB(255, 0, 255), 1, 8, 0);
			}
			if (not(character_box.in_left_strip) or not(character_box.in_right_strip))
			{
				cv::rectangle(visualise_image, cv::Point(character_box.left_most - 1, character_box.top_most - 1), cv::Point(character_box.right_most + 1, character_box.bottom_most + 1), CV_RGB(255, 0, 0), 1, 8, 0);
			}
		}
#endif // DEBUG
	}

	number_of_chars = candidate_characters.size();
	candidate_characters.clear();
#ifdef DEBUG
	cv::imwrite("d:/ocr/Visualisation.png", visualise_image);
	cv::imwrite("d:/ocr/visited.png", visited);
	std::cout << number_of_chars << " detected" << std::endl;
	std::cout << "Score" << score / number_of_chars << std::endl;
#endif // DEBUG
	if (number_of_chars == 0)
	{
		return 100;
	}
	return score / number_of_chars;
}



void OCRTemplate::grow_region(Fragment &image, cv::Mat &visited, int seed_row, int seed_col, SearchBox &box)
{
#ifdef DEBUG
	//std::cout << row << " " << col << " " << left_most << " " << right_most << " " << image.last_pixel[row] << " " << image.first_pixel[row] << std::endl;
#endif // DEBUG

	typedef std::pair<int, int> pixel_coord;
	std::queue<pixel_coord> pixel_queue{};

	pixel_queue.push(pixel_coord(seed_row, seed_col));
	while (not(pixel_queue.empty()))
	{
		int row = pixel_queue.front().first;
		int col = pixel_queue.front().second;
		pixel_queue.pop();

		if (between(0, row, image.image.rows) and between(image.first_pixel[row], col, image.last_pixel[row])
			and image(row, col) == 0 and not(visited.at<uchar>(row, col)))
		{
			visited.at<uchar>(row, col) = 1;
			box.in_left_strip |= col < image.centre[row];
			box.in_right_strip |= col >= image.centre[row];
			box.right_most = MAX(box.right_most, col);
			box.left_most = MIN(box.left_most, col);
			box.top_most = MIN(box.top_most, row);
			box.bottom_most = MAX(box.bottom_most, row);

			pixel_queue.push(pixel_coord(row, col + 1));//Right
			pixel_queue.push(pixel_coord(row, col - 1));//Left
			pixel_queue.push(pixel_coord(row + 1, col));//Down
			pixel_queue.push(pixel_coord(row - 1, col));//Up
		}
	}
}




void OCRTemplate::pad(cv::Mat &image)
{
	//Pad to same size as largest character
	if (image.cols < largest_width)
	{
		cv::Mat side_padding(image.rows, ceil((largest_width - image.cols) / 2.0), image.type(), cv::Scalar(255));
		cv::hconcat(side_padding, image, image);
		cv::hconcat(image, side_padding, image);
	}
	if (image.rows < largest_height)
	{
		cv::Mat top_padding(ceil((largest_height - image.rows) / 2.0), image.cols, image.type(), cv::Scalar(255));
		cv::vconcat(top_padding, image, image);
		cv::vconcat(image, top_padding, image);
	}
}

bool OCRTemplate::between(double a, double b, double c)
{
	return ((a <= b) and (b < c));// between a,b,c true iff a <= b < c
}

std::vector<SearchBox> OCRTemplate::get_candidate_characters(Fragment & combined_image)
{
	//Checks if each pixel has been visited yet
	cv::Mat visited(combined_image.image.rows, combined_image.image.cols, combined_image.image.type(), 0.0);

	//Threshold
	Fragment combined_image_thresholded = combined_image.get_thresholded();

#ifdef DEBUG
	cv::imwrite("D:/ocr/stitch.png", combined_image_thresholded.image);////////////////
	cv::imwrite("d:/ocr/visitedbefore.png", visited);////////
	cv::Mat visualise_image{ combined_image_thresholded.image };
	cvtColor(visualise_image, visualise_image, cv::COLOR_GRAY2RGB);
	for (int row = 0; row < combined_image.image.rows; row++)
	{
		cv::rectangle(visualise_image, cv::Point(combined_image.centre[row], row), cv::Point(combined_image.centre[row], row), CV_RGB(0, 255, 255), 1, 8, 0);
	}
#endif//DEBUG
	std::vector<SearchBox> candidate_characters{};
	//For each line of text
	for (int row = 0; row < combined_image_thresholded.image.rows; row++)
	{
		//Check either side of boundary
		for (int col = combined_image.centre[row] - 2; col < combined_image.centre[row] + 2; col++)
		{
			//If unvisited black pixel found
			if (combined_image_thresholded(row, col) == 0 and not(visited.at<uchar>(row, col)))
			{
				//Floodfill area
				SearchBox component_box(combined_image_thresholded.image.cols, 0, combined_image_thresholded.image.rows, 0);
				grow_region(combined_image_thresholded, visited, row, col, component_box);
				//If not too big
				if (not(component_box.width() >(1.25)*largest_width) and not(component_box.height() > (1.25)*largest_height))
				{
					//Check overlap with previous character
					if (candidate_characters.size() > 0 and component_box.top_most <= candidate_characters[candidate_characters.size() - 1].bottom_most + 2)
					{
						//Try to merge
						SearchBox new_box = component_box + candidate_characters[candidate_characters.size() - 1];
						if (not(new_box.width() > (1.25)*largest_width) and not(new_box.height() > (1.25)*largest_height))
						{
							candidate_characters[candidate_characters.size() - 1] = new_box;
						}
					}
					//If no overlap add to stack
					else
					{
						candidate_characters.emplace_back(component_box);
					}
				}
#ifdef DEBUG		//Visualise too large components
				else
				{
					cv::rectangle(visualise_image, cv::Point(component_box.left_most - 1, component_box.top_most - 1), cv::Point(component_box.right_most + 1, component_box.bottom_most + 1), CV_RGB(255, 255, 0), 1, 8, 0);
				}
#endif
			}
		}
	}

#ifdef DEBUG
	//For each candidate character
	for (SearchBox character_box : candidate_characters)
	{
		if (character_box.width() > (0.5)*smallest_width and character_box.height() > (0.75)*smallest_height//Check dimensions arent too small for detection
			and character_box.in_left_strip and character_box.in_right_strip)//Check components cross boundary
		{
			cv::rectangle(visualise_image, cv::Point(character_box.left_most - 1, character_box.top_most - 1), cv::Point(character_box.right_most + 1, character_box.bottom_most + 1), CV_RGB(0, 255, 0), 1, 8, 0);
		}
		else
		{
			if (character_box.width() <= (3.0 / 4)*smallest_width or character_box.height() <= (3.0 / 4)*smallest_height)
			{
				cv::rectangle(visualise_image, cv::Point(character_box.left_most - 1, character_box.top_most - 1), cv::Point(character_box.right_most + 1, character_box.bottom_most + 1), CV_RGB(255, 0, 255), 1, 8, 0);
			}
			if (not(character_box.in_left_strip) or not(character_box.in_right_strip))
			{
				cv::rectangle(visualise_image, cv::Point(character_box.left_most - 1, character_box.top_most - 1), cv::Point(character_box.right_most + 1, character_box.bottom_most + 1), CV_RGB(255, 0, 0), 1, 8, 0);
			}
		}
	}
#endif // DEBUG
	
	return candidate_characters;
}





SearchBox SearchBox::operator+(SearchBox & box2)
{
	SearchBox new_box{ MIN(left_most,box2.left_most),
		MAX(right_most,box2.right_most),
		MIN(top_most,box2.top_most),
		MAX(bottom_most,box2.bottom_most) };
	new_box.in_left_strip = in_left_strip or box2.in_left_strip;
	new_box.in_right_strip = in_right_strip or box2.in_right_strip;
	return new_box;
}

void SearchBox::operator+=(SearchBox & box2)
{
	left_most = MIN(left_most, box2.left_most);
	right_most = MAX(right_most, box2.right_most);
	top_most = MIN(top_most, box2.top_most);
	bottom_most = MAX(bottom_most, box2.bottom_most);
	in_left_strip |= box2.in_left_strip;
	in_right_strip |= box2.in_right_strip;

}