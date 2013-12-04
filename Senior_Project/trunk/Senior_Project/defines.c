//defines.c

#include <stdio.h>
#include <stdlib.h>
#include <avr/eeprom.h>
#include "defines.h"
#include "Motor.h"

volatile signed int Axis1_x_coord=0;
volatile signed int  Axis1_y_coord=0;
volatile signed int  Axis2_x_coord=0;
volatile signed int  Axis2_y_coord=0;
volatile signed int  Axis3_x_coord=0;
volatile signed int  Axis3_y_coord=0;
volatile signed int  Axis1_x_coord_old=0;
volatile signed int  Axis1_y_coord_old=0;
volatile int  Controller_state = 1;
volatile int  Coordinates_updated = 0;
volatile int  Coordinates_verified = 0;
volatile int  Receiving_Coords = 0;
volatile int  Coordinates_correct = 0;
volatile int  parsed_coord=0;
volatile int  state1_hysterisis_delay = 0;
volatile int  state2_hysterisis_delay = 0;
volatile int  PWM1A = 0;
volatile int  PWM1B = 0;
volatile int  PWM2A = 0;
volatile int  PWM2B = 0;
volatile char  buffer[10];       //Used to Store ADC value readings from itoa() of ADC register
volatile char  *clr_screen = "^\033[2J\033[0;0H";
volatile char  *Init_statement = "^\033[2J\033[0;0HWelcome to Crazy Numerical Control 3000\r\nWhere All Of Your Dreams Will Come True\r\n.......................................\r\n\r\n\r\n";
volatile char  *Coord_request_x = "Please Input The Desired X Coordinate:";
volatile char  *Coord_request_y = "Please Input The Desired Y Coordinate:";
volatile char  *Verify_coords1 = "You input the Coordinates (";
volatile char  *Verify_coords2 = ",";
volatile char  *Verify_coords3 = "). Is this correct? (Y/N)";
volatile char  *Verify_coords4 = "Moving To Desired Coordinates...";
volatile char  *Verify_coords5 = "Please Renter Coordinates:";
volatile char  *newline = "\r\n";
volatile char  *clearline = "\r";
volatile int motor_movement_distance=0;
volatile int motor_dir = POSITIVE;
volatile int motor_moving = NO;
volatile int  move_motor = NO;
volatile int motor_step_count = 0;
volatile int ADC_count = 0;
volatile int count = 0;
volatile int iter = 0;
volatile int state1 = POS_CUR;
volatile int state2 = POS_CUR;
volatile char data;
volatile uint16_t ADCval=0;
volatile uint16_t ADCval1=0;
volatile uint16_t ADCval2=0;
volatile uint16_t ADCval3=0;
volatile uint16_t ADCval4=0;
volatile uint16_t ADC_avg=0;
volatile uint8_t  SRAMchar;
volatile float SRAMfloat;
volatile uint16_t SRAMint;
volatile uint8_t  SRAMstring[10];
volatile char mydatabuffer[11][10];
volatile float EEMEM x_coord_data[ 629 ] = {6.000, 6.000, 6.000, 5.999, 5.998, 5.998, 5.996, 5.995, 5.994, 5.992, 5.990, 5.988, 5.986, 5.983, 5.980, 5.978, 5.974, 5.971, 5.968, 5.964, 5.960, 5.956, 5.952, 5.947, 5.943, 5.938, 5.933, 5.928, 5.922, 5.916, 5.911, 5.905, 5.898, 5.892, 5.886, 5.879, 5.872, 5.865, 5.857, 5.850, 5.842, 5.834, 5.826, 5.818, 5.810, 5.801, 5.792, 5.783, 5.774, 5.765, 5.755, 5.745, 5.736, 5.726, 5.715, 5.705, 5.695, 5.684, 5.673, 5.662, 5.651, 5.639, 5.628, 5.616, 5.604, 5.592, 5.580, 5.568, 5.555, 5.542, 5.530, 5.517, 5.504, 5.490, 5.477, 5.463, 5.450, 5.436, 5.422, 5.408, 5.393, 5.379, 5.364, 5.350, 5.335, 5.320, 5.305, 5.290, 5.274, 5.259, 5.243, 5.227, 5.212, 5.196, 5.180, 5.163, 5.147, 5.131, 5.114, 5.097, 5.081, 5.064, 5.047, 5.030, 5.012, 4.995, 4.978, 4.960, 4.943, 4.925, 4.907, 4.889, 4.871, 4.853, 4.835, 4.817, 4.799, 4.780, 4.762, 4.743, 4.725, 4.706, 4.687, 4.668, 4.650, 4.631, 4.612, 4.593, 4.573, 4.554, 4.535, 4.516, 4.496, 4.477, 4.458, 4.438, 4.418, 4.399, 4.379, 4.360, 4.340, 4.320, 4.300, 4.281, 4.261, 4.241, 4.221, 4.201, 4.181, 4.161, 4.141, 4.122, 4.102, 4.082, 4.062, 4.042, 4.022, 4.002, 3.982, 3.962, 3.942, 3.922, 3.902, 3.882, 3.862, 3.842, 3.822, 3.802, 3.782, 3.762, 3.742, 3.722, 3.703, 3.683, 3.663, 3.644, 3.624, 3.604, 3.585, 3.565, 3.546, 3.526, 3.507, 3.487, 3.468, 3.449, 3.430, 3.410, 3.391, 3.372, 3.353, 3.335, 3.316, 3.297, 3.278, 3.260, 3.241, 3.223, 3.204, 3.186, 3.168, 3.150, 3.132, 3.114, 3.096, 3.078, 3.060, 3.043, 3.025, 3.008, 2.990, 2.973, 2.956, 2.939, 2.922, 2.905, 2.889, 2.872, 2.856, 2.839, 2.823, 2.807, 2.791, 2.775, 2.759, 2.744, 2.728, 2.713, 2.698, 2.682, 2.667, 2.653, 2.638, 2.623, 2.609, 2.595, 2.580, 2.566, 2.553, 2.539, 2.525, 2.512, 2.498, 2.485, 2.472, 2.460, 2.447, 2.434, 2.422, 2.410, 2.398, 2.386, 2.374, 2.363, 2.351, 2.340, 2.329, 2.318, 2.307, 2.297, 2.286, 2.276, 2.266, 2.256, 2.246, 2.237, 2.227, 2.218, 2.209, 2.200, 2.192, 2.183, 2.175, 2.167, 2.159, 2.151, 2.144, 2.136, 2.129, 2.122, 2.116, 2.109, 2.103, 2.096, 2.090, 2.084, 2.079, 2.073, 2.068, 2.063, 2.058, 2.053, 2.049, 2.045, 2.041, 2.037, 2.033, 2.029, 2.026, 2.023, 2.020, 2.017, 2.015, 2.012, 2.010, 2.008, 2.007, 2.005, 2.004, 2.003, 2.002, 2.001, 2.000, 2.000, 2.000, 2.000, 2.000, 2.001, 2.001, 2.002, 2.003, 2.005, 2.006, 2.008, 2.010, 2.012, 2.014, 2.016, 2.019, 2.022, 2.025, 2.028, 2.032, 2.035, 2.039, 2.043, 2.048, 2.052, 2.057, 2.061, 2.066, 2.072, 2.077, 2.083, 2.088, 2.094, 2.101, 2.107, 2.113, 2.120, 2.127, 2.134, 2.141, 2.149, 2.157, 2.164, 2.173, 2.181, 2.189, 2.198, 2.206, 2.215, 2.225, 2.234, 2.243, 2.253, 2.263, 2.273, 2.283, 2.293, 2.304, 2.314, 2.325, 2.336, 2.348, 2.359, 2.370, 2.382, 2.394, 2.406, 2.418, 2.430, 2.443, 2.455, 2.468, 2.481, 2.494, 2.508, 2.521, 2.534, 2.548, 2.562, 2.576, 2.590, 2.604, 2.619, 2.633, 2.648, 2.663, 2.678, 2.693, 2.708, 2.723, 2.739, 2.754, 2.770, 2.786, 2.802, 2.818, 2.834, 2.850, 2.867, 2.883, 2.900, 2.917, 2.934, 2.951, 2.968, 2.985, 3.002, 3.019, 3.037, 3.055, 3.072, 3.090, 3.108, 3.126, 3.144, 3.162, 3.180, 3.198, 3.217, 3.235, 3.254, 3.272, 3.291, 3.310, 3.329, 3.347, 3.366, 3.385, 3.404, 3.424, 3.443, 3.462, 3.481, 3.501, 3.520, 3.539, 3.559, 3.578, 3.598, 3.618, 3.637, 3.657, 3.677, 3.696, 3.716, 3.736, 3.756, 3.776, 3.796, 3.815, 3.835, 3.855, 3.875, 3.895, 3.915, 3.935, 3.955, 3.975, 3.995, 4.015, 4.035, 4.055, 4.075, 4.095, 4.115, 4.135, 4.155, 4.175, 4.195, 4.215, 4.235, 4.255, 4.274, 4.294, 4.314, 4.334, 4.353, 4.373, 4.393, 4.412, 4.432, 4.451, 4.471, 4.490, 4.510, 4.529, 4.548, 4.567, 4.586, 4.606, 4.625, 4.644, 4.662, 4.681, 4.700, 4.719, 4.737, 4.756, 4.774, 4.793, 4.811, 4.829, 4.848, 4.866, 4.884, 4.902, 4.919, 4.937, 4.955, 4.972, 4.990, 5.007, 5.024, 5.041, 5.058, 5.075, 5.092, 5.109, 5.125, 5.142, 5.158, 5.174, 5.191, 5.207, 5.222, 5.238, 5.254, 5.269, 5.285, 5.300, 5.315, 5.330, 5.345, 5.360, 5.374, 5.389, 5.403, 5.417, 5.431, 5.445, 5.459, 5.473, 5.486, 5.499, 5.513, 5.526, 5.538, 5.551, 5.564, 5.576, 5.588, 5.600, 5.612, 5.624, 5.636, 5.647, 5.658, 5.669, 5.680, 5.691, 5.702, 5.712, 5.722, 5.732, 5.742, 5.752, 5.762, 5.771, 5.780, 5.789, 5.798, 5.807, 5.815, 5.824, 5.832, 5.840, 5.847, 5.855, 5.862, 5.870, 5.877, 5.883, 5.890, 5.896, 5.903, 5.909, 5.915, 5.920, 5.926, 5.931, 5.936, 5.941, 5.946, 5.950, 5.955, 5.959, 5.963, 5.967, 5.970, 5.973, 5.977, 5.980, 5.982, 5.985, 5.987, 5.989, 5.991, 5.993, 5.995, 5.996, 5.997, 5.998, 5.999, 5.999, 6.000, 6.000};
volatile float EEMEM y_coord_data[ 629 ] = {6.000, 6.020, 6.040, 6.060, 6.080, 6.100, 6.120, 6.140, 6.160, 6.180, 6.200, 6.220, 6.239, 6.259, 6.279, 6.299, 6.319, 6.338, 6.358, 6.378, 6.397, 6.417, 6.436, 6.456, 6.475, 6.495, 6.514, 6.533, 6.553, 6.572, 6.591, 6.610, 6.629, 6.648, 6.667, 6.686, 6.705, 6.723, 6.742, 6.760, 6.779, 6.797, 6.816, 6.834,6.852, 6.870, 6.888, 6.906, 6.924, 6.941, 6.959, 6.976, 6.994, 7.011, 7.028, 7.045, 7.062, 7.079, 7.096, 7.113, 7.129, 7.146, 7.162, 7.178, 7.194, 7.210, 7.226, 7.242, 7.258, 7.273, 7.288, 7.304, 7.319, 7.334, 7.349, 7.363, 7.378, 7.392, 7.407, 7.421, 7.435, 7.449, 7.462, 7.476, 7.489, 7.503, 7.516, 7.529, 7.541, 7.554, 7.567, 7.579, 7.591, 7.603, 7.615, 7.627, 7.638, 7.650, 7.661, 7.672, 7.683, 7.694, 7.704, 7.715, 7.725, 7.735, 7.745, 7.754, 7.764, 7.773, 7.782, 7.791, 7.800, 7.809, 7.817, 7.826, 7.834, 7.842, 7.849, 7.857, 7.864, 7.871, 7.878, 7.885, 7.892, 7.898, 7.904, 7.910, 7.916, 7.922, 7.927, 7.932, 7.937, 7.942, 7.947, 7.951, 7.956, 7.960, 7.964, 7.967, 7.971, 7.974, 7.977, 7.980, 7.983, 7.985, 7.988, 7.990, 7.992, 7.993, 7.995, 7.996, 7.997, 7.998, 7.999, 8.000, 8.000, 8.000, 8.000, 8.000, 7.999, 7.998, 7.998, 7.996, 7.995, 7.994, 7.992, 7.990, 7.988, 7.986, 7.983, 7.981, 7.978, 7.975, 7.971, 7.968, 7.964, 7.960, 7.956, 7.952, 7.948, 7.943, 7.938, 7.933, 7.928, 7.923, 7.917, 7.911, 7.905, 7.899, 7.893, 7.886, 7.879, 7.872, 7.865, 7.858, 7.850, 7.843, 7.835, 7.827, 7.819, 7.810, 7.802, 7.793, 7.784, 7.775, 7.765, 7.756, 7.746, 7.736, 7.726, 7.716, 7.706, 7.695, 7.685, 7.674, 7.663, 7.652, 7.640, 7.629, 7.617, 7.605, 7.593, 7.581, 7.569, 7.556, 7.544, 7.531, 7.518, 7.505, 7.491, 7.478, 7.464, 7.451, 7.437, 7.423, 7.409, 7.395, 7.380, 7.366, 7.351, 7.336, 7.321, 7.306, 7.291, 7.276, 7.260, 7.244, 7.229, 7.213, 7.197, 7.181, 7.165, 7.148, 7.132, 7.115, 7.099, 7.082, 7.065, 7.048, 7.031, 7.014, 6.997, 6.979, 6.962, 6.944, 6.926, 6.909, 6.891, 6.873, 6.855, 6.837, 6.818, 6.800, 6.782, 6.763, 6.745, 6.726, 6.708, 6.689, 6.670, 6.651, 6.632, 6.613, 6.594, 6.575, 6.556, 6.537, 6.517, 6.498, 6.478, 6.459, 6.440, 6.420, 6.400, 6.381, 6.361, 6.342, 6.322, 6.302, 6.282, 6.262, 6.243, 6.223, 6.203, 6.183, 6.163, 6.143, 6.123, 6.103, 6.083, 6.063, 6.043, 6.023, 6.003, 5.983, 5.963, 5.943, 5.923, 5.903, 5.883, 5.863, 5.843, 5.823, 5.804, 5.784, 5.764, 5.744, 5.724, 5.704, 5.685, 5.665, 5.645, 5.625, 5.606, 5.586, 5.567, 5.547, 5.528, 5.508, 5.489, 5.470, 5.450, 5.431, 5.412, 5.393, 5.374, 5.355, 5.336, 5.317, 5.298, 5.280, 5.261, 5.243, 5.224, 5.206, 5.187, 5.169, 5.151, 5.133, 5.115, 5.097, 5.079, 5.062, 5.044, 5.026, 5.009, 4.992, 4.974, 4.957, 4.940, 4.923, 4.907, 4.890, 4.873, 4.857, 4.841, 4.824, 4.808, 4.792, 4.776, 4.761, 4.745, 4.729, 4.714, 4.699, 4.684, 4.669, 4.654, 4.639, 4.624, 4.610, 4.596, 4.582, 4.568, 4.554, 4.540, 4.526, 4.513, 4.500, 4.486, 4.473, 4.461, 4.448, 4.435, 4.423, 4.411, 4.399, 4.387, 4.375, 4.363, 4.352, 4.341, 4.330, 4.319, 4.308, 4.297, 4.287, 4.277, 4.267, 4.257, 4.247, 4.238, 4.228, 4.219, 4.210, 4.201, 4.193, 4.184, 4.176, 4.168, 4.160, 4.152, 4.144, 4.137, 4.130, 4.123, 4.116, 4.109, 4.103, 4.097, 4.091, 4.085, 4.079, 4.074, 4.068, 4.063, 4.058, 4.054, 4.049, 4.045, 4.041, 4.037, 4.033, 4.030, 4.026, 4.023, 4.020, 4.018, 4.015, 4.013, 4.010, 4.009, 4.007, 4.005, 4.004, 4.003, 4.002, 4.001, 4.001, 4.000, 4.000, 4.000, 4.000, 4.001, 4.001, 4.002, 4.003, 4.005, 4.006, 4.008, 4.010, 4.012, 4.014, 4.016, 4.019, 4.022, 4.025, 4.028, 4.031, 4.035, 4.039, 4.043, 4.047, 4.052, 4.056, 4.061, 4.066, 4.071, 4.077, 4.082, 4.088, 4.094, 4.100, 4.106, 4.113, 4.120, 4.127, 4.134, 4.141, 4.148, 4.156, 4.164, 4.172, 4.180, 4.188, 4.197, 4.206, 4.215, 4.224, 4.233, 4.243, 4.252, 4.262, 4.272, 4.282, 4.292, 4.303, 4.314, 4.324, 4.335, 4.347, 4.358, 4.369, 4.381, 4.393, 4.405, 4.417, 4.429, 4.442, 4.454, 4.467, 4.480, 4.493, 4.506, 4.520, 4.533, 4.547, 4.561, 4.575, 4.589, 4.603, 4.618, 4.632, 4.647, 4.662, 4.676, 4.692, 4.707, 4.722, 4.737, 4.753, 4.769, 4.785, 4.801, 4.817, 4.833, 4.849, 4.865, 4.882, 4.899, 4.915, 4.932, 4.949, 4.966, 4.983, 5.001, 5.018, 5.036, 5.053, 5.071, 5.089, 5.106, 5.124, 5.142, 5.160, 5.179, 5.197, 5.215, 5.234, 5.252, 5.271, 5.289, 5.308, 5.327, 5.346, 5.365, 5.384, 5.403, 5.422, 5.441, 5.460, 5.480, 5.499, 5.518, 5.538, 5.557, 5.577, 5.596, 5.616, 5.636, 5.655, 5.675, 5.695, 5.715, 5.734, 5.754, 5.774, 5.794, 5.814, 5.834, 5.854, 5.874, 5.894, 5.914, 5.934, 5.954, 5.974, 5.994};

struct MOTOR_T MOTOR1_X = {POSITIVE, 0, NO, FULLSTEP};

struct MOTOR_T MOTOR2_Y = {POSITIVE, 0, NO, FULLSTEP};

struct MOTOR_T MOTOR3_Z = {POSITIVE, 0, NO, FULLSTEP};
	

