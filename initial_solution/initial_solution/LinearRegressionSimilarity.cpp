#include "stdafx.h"
#include "LinearRegressionSimilarity.h"
#define DEBUG

std::vector<double> LinearRegressionSimilarity::process(Fragment & left_image, Fragment & right_image)
{
	//Combine images
	int best_height = best_height_match(left_image, right_image);
	Fragment combined_image{ combine(left_image,right_image,best_height) };
	//Threshold
	Fragment combined_image_thresholded = combined_image.get_thresholded();

	//Get lines
	bool *is_text_line{ new bool[combined_image_thresholded.image.rows] };
	combined_image_thresholded.get_text_lines(is_text_line);


	std::vector<double> total_values(filter.number_filters);
	int number_values{ 0 };
	//For each line
	for (int row = 0; row < combined_image_thresholded.image.rows; row++)
	{
		if (is_text_line[row])
		{
			int top{ row };
			while (is_text_line[row])
			{
				++row;
			}
			int bottom{ row };
			//Make box centred on centre of line
			int adjusted_row = MAX(top, (top) + best_height);
			int centre = left_image.last_pixel[adjusted_row];
			if (bottom - top > 5)
			{
				int width = 10;// floor((bottom - top) / 2);
				//std::cout << centre << " " << width << " " << top << " " << bottom << std::endl;

				cv::Mat feature(combined_image.image,
					cv::Range(top, MIN((bottom + 1), combined_image.image.rows)), // rows
					cv::Range(centre - width, centre + width + 1));// cols



				std::vector<double> feature_values{ filter.get_values(feature) };
				for (int i = 0; i < feature_values.size(); ++i)
				{
					total_values[i] += feature_values[i];
				}
				number_values++;
			}

			
		}
	}
	for (int i = 0; i < total_values.size(); ++i)
	{
		total_values[i] = total_values[i]/ number_values;
	}
	//Get filtered values
	return filter.get_values(combined_image.image);
}


long double LinearRegressionSimilarity::cost(Fragment & left_image, Fragment & right_image)
{
	std::vector<double> processed_vars{ process(left_image, right_image) };
	alglib::real_1d_array vars{};
	vars.setlength(processed_vars.size());
	for (size_t i = 0; i < processed_vars.size(); i++)
	{
		vars[i] = processed_vars[i];
	}

	return alglib::lrprocess(model, vars);
}

LinearRegressionSimilarity::LinearRegressionSimilarity(Filter &filter_,std::string dataset):
filter(filter_)
{
	//Train model on dataset
	//Read dataset file
	std::vector<std::vector<double>> data{};
	std::ifstream file(dataset);
	std::string root;
	std::string document;
	std::getline(file, root);
	//For each set of data
	while (std::getline(file, document))
	{
		//Load
		Solution images{ load(root + "/" + document  ,document) };
		for (int index{ 0 }; index < images.size(); ++index)
		{
			if (index < images.size() - 1)
			{
				//Process
				std::vector<double> filtered{ process(images[index],images[index+1]) };
				//append response (0 for correct) 
				filtered.emplace_back(0);
				//add to data
				data.emplace_back(filtered);
			}
			//Add a negative result
			//Get random index
			int right_index = random::rand_int(images.size() - 1);
			//Process
			std::vector<double> filtered_negative{ process(images[index],images[right_index]) };
			//append response (1 for incorrect) 
			filtered_negative.emplace_back(1);
			//add to data
			data.emplace_back(filtered_negative);
		}
		
	}
	

	//Plug all into linear regression model
	alglib::real_2d_array xy{};
	int number_data_points{ (int)data.size() };
	int num_cols{ (int)data[0].size() };
	int number_vars{ num_cols - 1 };

	xy.setlength(number_data_points, num_cols);
	//Transfer data
#ifdef DEBUG

	std::ofstream f("data.txt");	
#endif

	for (size_t i = 0; i < number_data_points; i++)
	{
		for (size_t j = 0; j < num_cols; j++)
		{
#ifdef DEBUG
			if (f.is_open())
				f << data[i][j] << " ";
#endif
			xy(i, j) = data[i][j];
		}
#ifdef DEBUG
		if (f.is_open())
			f << "\n";
#endif
	}
	alglib::ae_int_t info;
	alglib::lrreport rep;
	lrbuildz(xy, number_data_points, number_vars, info, model, rep);
	
#ifdef DEBUG
	f.close();
	alglib::ae_int_t nvars;
	alglib::real_1d_array c;
	printf("%d\n", int(info)); // EXPECTED: 1
	lrunpack(model, c, nvars);
	printf("%s\n", c.tostring(4).c_str()); 
#endif // DEBUG

	
}


LinearRegressionSimilarity::~LinearRegressionSimilarity()
{
}
