#include "helper/reader.h"
#include "helper/input_parser.h"
#include "helper/writer.h"
#include "../layers/convolution_layer/convolution.h"
#include "../layers/pooling_layer/pooling.h"
#include "../layers/fully_connected_layer/fully_connected.h"
#include "../layers/relu_layer/relu.h"
#include "../layers/eltwise_layer/eltwise.h"

#include <string.h>

using Eigen::Matrix;
using Eigen::RowMajor;

int main(int argc, char *argv[])
{
    if (argc == 2) {
        if (!strcmp(argv[1], "eigen") || !strcmp(argv[1], "gemmlowp")) {
            ;
        } else {
            cout << "ERROR --> usage: ./lpa_cnn.out <eigen> | <gemmlowp>" << endl;
            exit(1);
        }
    } else {
        cout << "ERROR --> usage: ./lpa_cnn.out <eigen> | <gemmlowp>" << endl;
        exit(1);
    }

    float gemm_time_total = 0.0;
    float run_time_total = 0.0;

    std::string mode = argv[1];

	const int im_height_1 = 224;
	const int im_width_1 = 224;
	const int im_depth_1 = 3;
	const int im_size_1 = im_height_1*im_width_1;
	
	const int k_num_1 = 96;
	const int k_size_1 = 49;
	const int stride_1 = 2;
	const int k_depth_1 = im_depth_1;
	
	const int p1_1 = 0;
	const int p2_1 = 0;
	
	const int output_height_1 = (im_height_1 + 2 * p1_1 - sqrt(k_size_1)) / stride_1 + 1;
	const int output_width_1 = (im_width_1 + 2 * p2_1 - sqrt(k_size_1)) / stride_1 + 1;
	const int output_size_1 = output_height_1 * output_width_1;
	
	MatrixXd conv1_weights = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/conv1_weights.csv");
	Map<MatrixXd> conv1_w(conv1_weights.data(), k_num_1, k_size_1 * k_depth_1);
	const float conv1_min = conv1_w.minCoeff();
	const float conv1_max = conv1_w.maxCoeff();
	
	MatrixXd conv1_result_params = load_csv_arma<MatrixXd>("../features/VGG_CNN_S/caffe/result_params.csv");
	const float conv1_result_min = conv1_result_params(0, 0);
	const float conv1_result_max = conv1_result_params(0, 1);
	
	MatrixXd conv1_biases = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/conv1_biases.csv");
	VectorXd conv1_b(Map<VectorXd>(conv1_biases.data(), conv1_biases.cols()*conv1_biases.rows()));
	
	const int f_1 = 2;
	const int s_1 = 3;
	std::string mode_1 = "max";
	
	const int pp1_1 = 0;
	const int pp2_1 = 0;
	
	const int im_height_2 = static_cast<int>(ceil(static_cast<float>(output_height_1 + 2 * pp1_1 - f_1 ) / s_1)) + 1;
	const int im_width_2 = static_cast<int>(ceil(static_cast<float>(output_width_1 + 2 * pp2_1 - f_1) / s_1)) + 1;
	const int im_depth_2 = k_num_1;
	const int im_size_2 = im_height_2 * im_width_2;
	
	const int k_num_2 = 256;
	const int k_size_2 = 25;
	const int stride_2 = 1;
	const int k_depth_2 = im_depth_2;
	
	const int p1_2 = 0;
	const int p2_2 = 0;
	
	const int output_height_2 = (im_height_2 + 2 * p1_2 - sqrt(k_size_2)) / stride_2 + 1;
	const int output_width_2 = (im_width_2 + 2 * p2_2 - sqrt(k_size_2)) / stride_2 + 1;
	const int output_size_2 = output_height_2 * output_width_2;
	
	MatrixXd conv2_weights = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/conv2_weights.csv");
	MatrixXd conv2_w = conv2_weights;
	const float conv2_min = conv2_w.minCoeff();
	const float conv2_max = conv2_w.maxCoeff();
	
	MatrixXd conv2_result_params = load_csv_arma<MatrixXd>("../features/VGG_CNN_S/caffe/result_params.csv");
	const float conv2_result_min = conv2_result_params(1, 0);
	const float conv2_result_max = conv2_result_params(1, 1);
	
	MatrixXd conv2_biases = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/conv2_biases.csv");
	VectorXd conv2_b(Map<VectorXd>(conv2_biases.data(), conv2_biases.cols()*conv2_biases.rows()));
	
	const int f_2 = 2;
	const int s_2 = 2;
	std::string mode_2 = "max";
	
	const int pp1_2 = 0;
	const int pp2_2 = 0;
	
	const int im_height_3 = static_cast<int>(ceil(static_cast<float>(output_height_2 + 2 * pp1_2 - f_2 ) / s_2)) + 1;
	const int im_width_3 = static_cast<int>(ceil(static_cast<float>(output_width_2 + 2 * pp2_2 - f_2) / s_2)) + 1;
	const int im_depth_3 = k_num_2;
	const int im_size_3 = im_height_3 * im_width_3;
	
	const int k_num_3 = 512;
	const int k_size_3 = 9;
	const int stride_3 = 1;
	const int k_depth_3 = im_depth_3;
	
	const int p1_3 = 1;
	const int p2_3 = 1;
	
	const int output_height_3 = (im_height_3 + 2 * p1_3 - sqrt(k_size_3)) / stride_3 + 1;
	const int output_width_3 = (im_width_3 + 2 * p2_3 - sqrt(k_size_3)) / stride_3 + 1;
	const int output_size_3 = output_height_3 * output_width_3;
	
	MatrixXd conv3_weights = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/conv3_weights.csv");
	MatrixXd conv3_w = conv3_weights;
	const float conv3_min = conv3_w.minCoeff();
	const float conv3_max = conv3_w.maxCoeff();
	
	MatrixXd conv3_result_params = load_csv_arma<MatrixXd>("../features/VGG_CNN_S/caffe/result_params.csv");
	const float conv3_result_min = conv3_result_params(2, 0);
	const float conv3_result_max = conv3_result_params(2, 1);
	
	MatrixXd conv3_biases = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/conv3_biases.csv");
	VectorXd conv3_b(Map<VectorXd>(conv3_biases.data(), conv3_biases.cols()*conv3_biases.rows()));
	
	const int im_height_4 = output_height_3;
	const int im_width_4 = output_width_3;
	const int im_depth_4 = k_num_3;
	const int im_size_4 = im_height_4 * im_width_4;
	
	const int k_num_4 = 512;
	const int k_size_4 = 9;
	const int stride_4 = 1;
	const int k_depth_4 = im_depth_4;
	
	const int p1_4 = 1;
	const int p2_4 = 1;
	
	const int output_height_4 = (im_height_4 + 2 * p1_4 - sqrt(k_size_4)) / stride_4 + 1;
	const int output_width_4 = (im_width_4 + 2 * p2_4 - sqrt(k_size_4)) / stride_4 + 1;
	const int output_size_4 = output_height_4 * output_width_4;
	
	MatrixXd conv4_weights = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/conv4_weights.csv");
	MatrixXd conv4_w = conv4_weights;
	const float conv4_min = conv4_w.minCoeff();
	const float conv4_max = conv4_w.maxCoeff();
	
	MatrixXd conv4_result_params = load_csv_arma<MatrixXd>("../features/VGG_CNN_S/caffe/result_params.csv");
	const float conv4_result_min = conv4_result_params(3, 0);
	const float conv4_result_max = conv4_result_params(3, 1);
	
	MatrixXd conv4_biases = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/conv4_biases.csv");
	VectorXd conv4_b(Map<VectorXd>(conv4_biases.data(), conv4_biases.cols()*conv4_biases.rows()));
	
	const int im_height_5 = output_height_4;
	const int im_width_5 = output_width_4;
	const int im_depth_5 = k_num_4;
	const int im_size_5 = im_height_5 * im_width_5;
	
	const int k_num_5 = 512;
	const int k_size_5 = 9;
	const int stride_5 = 1;
	const int k_depth_5 = im_depth_5;
	
	const int p1_5 = 1;
	const int p2_5 = 1;
	
	const int output_height_5 = (im_height_5 + 2 * p1_5 - sqrt(k_size_5)) / stride_5 + 1;
	const int output_width_5 = (im_width_5 + 2 * p2_5 - sqrt(k_size_5)) / stride_5 + 1;
	const int output_size_5 = output_height_5 * output_width_5;
	
	MatrixXd conv5_weights = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/conv5_weights.csv");
	MatrixXd conv5_w = conv5_weights;
	const float conv5_min = conv5_w.minCoeff();
	const float conv5_max = conv5_w.maxCoeff();
	
	MatrixXd conv5_result_params = load_csv_arma<MatrixXd>("../features/VGG_CNN_S/caffe/result_params.csv");
	const float conv5_result_min = conv5_result_params(4, 0);
	const float conv5_result_max = conv5_result_params(4, 1);
	
	MatrixXd conv5_biases = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/conv5_biases.csv");
	VectorXd conv5_b(Map<VectorXd>(conv5_biases.data(), conv5_biases.cols()*conv5_biases.rows()));
	
	const int f_3 = 2;
	const int s_3 = 3;
	std::string mode_3 = "max";
	
	const int pp1_3 = 0;
	const int pp2_3 = 0;
	
	MatrixXd fc6_weights = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/fc6_weights.csv");
	
	MatrixXd fc6_biases = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/fc6_biases.csv");
	VectorXd fc6_b(Map<VectorXd>(fc6_biases.data(), fc6_biases.cols()*fc6_biases.rows()));
	
	MatrixXd fc7_weights = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/fc7_weights.csv");
	
	MatrixXd fc7_biases = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/fc7_biases.csv");
	VectorXd fc7_b(Map<VectorXd>(fc7_biases.data(), fc7_biases.cols()*fc7_biases.rows()));
	
	MatrixXd fc8_weights = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/fc8_weights.csv");
	
	MatrixXd fc8_biases = load_csv_arma<MatrixXd>("../weights/VGG_CNN_S/fc8_biases.csv");
	VectorXd fc8_b(Map<VectorXd>(fc8_biases.data(), fc8_biases.cols()*fc8_biases.rows()));
	
	const int im_num = 1000;
	
	ifstream infile;
	infile.open("../inputs/VGG_CNN_S/production/imagenet_img_norm_1000.csv");
	
    for(int i=0; i < im_num; ++i)
    {
        cout << i << endl;

		MatrixXd line = load_csv<MatrixXd>(infile);
		
        MatrixXd img;
        img = line.block<1,im_size_1*im_depth_1>(0,1);

        MatrixXd image = Map<Matrix<double, im_depth_1, im_size_1, RowMajor>>(img.data());

        const float input_min = image.minCoeff();
        const float input_max = image.maxCoeff();

        clock_t run_time_start = clock();

		MatrixXd conv1;
		float gemm_time_1;
		float offline_time_1;
		std::tie(conv1, gemm_time_1, offline_time_1) = convolve(image, im_size_1, im_height_1, im_width_1, im_depth_1, k_size_1, stride_1, conv1_b, p1_1, p2_1, conv1_w, output_size_1, mode, conv1_min, conv1_max, input_min, input_max, conv1_result_min, conv1_result_max);
		
		MatrixXd relu1 = relu(conv1);
		
		MatrixXd pool1 = pool(relu1, f_1, s_1, output_width_1, output_height_1, pp1_1, pp2_1, mode_1);
		
		MatrixXd conv2;
		float gemm_time_2;
		float offline_time_2;
		std::tie(conv2, gemm_time_2, offline_time_2) = convolve(pool1, im_size_2, im_height_2, im_width_2, im_depth_2, k_size_2, stride_2, conv2_b, p1_2, p2_2, conv2_w, output_size_2, mode, conv2_min, conv2_max, input_min, input_max, conv2_result_min, conv2_result_max);
		
		MatrixXd relu2 = relu(conv2);
		
		MatrixXd pool2 = pool(relu2, f_2, s_2, output_width_2, output_height_2, pp1_2, pp2_2, mode_2);
		
		MatrixXd conv3;
		float gemm_time_3;
		float offline_time_3;
		std::tie(conv3, gemm_time_3, offline_time_3) = convolve(pool2, im_size_3, im_height_3, im_width_3, im_depth_3, k_size_3, stride_3, conv3_b, p1_3, p2_3, conv3_w, output_size_3, mode, conv3_min, conv3_max, input_min, input_max, conv3_result_min, conv3_result_max);
		
		MatrixXd relu3 = relu(conv3);
		
		MatrixXd conv4;
		float gemm_time_4;
		float offline_time_4;
		std::tie(conv4, gemm_time_4, offline_time_4) = convolve(relu3, im_size_4, im_height_4, im_width_4, im_depth_4, k_size_4, stride_4, conv4_b, p1_4, p2_4, conv4_w, output_size_4, mode, conv4_min, conv4_max, input_min, input_max, conv4_result_min, conv4_result_max);
		
		MatrixXd relu4 = relu(conv4);
		
		MatrixXd conv5;
		float gemm_time_5;
		float offline_time_5;
		std::tie(conv5, gemm_time_5, offline_time_5) = convolve(relu4, im_size_5, im_height_5, im_width_5, im_depth_5, k_size_5, stride_5, conv5_b, p1_5, p2_5, conv5_w, output_size_5, mode, conv5_min, conv5_max, input_min, input_max, conv5_result_min, conv5_result_max);
		
		MatrixXd relu5 = relu(conv5);
		
		MatrixXd pool5 = pool(relu5, f_3, s_3, output_width_5, output_height_5, pp1_3, pp2_3, mode_3);
		
		MatrixXd fc6 = fully_connect(pool5, pool5.rows(), fc6_weights, fc6_b);
		
		MatrixXd relu6 = relu(fc6);
		
		MatrixXd fc7 = fully_connect(relu6, relu6.rows(), fc7_weights, fc7_b);
		
		MatrixXd relu7 = relu(fc7);
		
		MatrixXd fc8 = fully_connect(relu7, relu7.rows(), fc8_weights, fc8_b);
		
        clock_t run_time_end = clock();

        float run_time = (float) (run_time_end-run_time_start) / CLOCKS_PER_SEC;
		run_time_total += (run_time - offline_time_1 - offline_time_2 - offline_time_3 - offline_time_4 - offline_time_5);
		gemm_time_total += 0.0 + gemm_time_1 + gemm_time_2 + gemm_time_3 + gemm_time_4 + gemm_time_5;
		
		std::string name_1 = "../features/VGG_CNN_S/" + mode + "/fc8_" + std::to_string(i) + ".csv";
		write_to_csv(name_1, fc8);
    }

    infile.close();

    cout << "-----------------------------" << endl;

    float avg_run_time = 0.0;
    avg_run_time = run_time_total / im_num;
    cout << "average online run time: " << avg_run_time << endl;

    float avg_gemm_time = 0.0;
    avg_gemm_time = gemm_time_total / im_num;
    cout << "average total time for GEMM: " << avg_gemm_time << endl;

    return 0;
}
