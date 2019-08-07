// MsAzKinSensor.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <k4a/k4a.h>
//#include <k4atypes.h>
#include <k4arecord/record.h>
#include <k4arecord/playback.h>

int main()
{
    std::cout << "Hello World!\n"; 
	uint32_t count = k4a_device_get_installed_count();
	std::cout << count;
	// Open the first plugged in Kinect device
	k4a_device_t device = NULL;
	k4a_device_open(K4A_DEVICE_DEFAULT, &device);

	// Get the size of the serial number
	size_t serial_size = 0;
	k4a_device_get_serialnum(device, NULL, &serial_size);

	// Allocate memory for the serial, then acquire it
	char *serial = (char*)(malloc(serial_size));
	k4a_device_get_serialnum(device, serial, &serial_size);
	printf("Opened device: %s\n", serial);
	free(serial);

	k4a_device_configuration_t config = K4A_DEVICE_CONFIG_INIT_DISABLE_ALL;
	config.camera_fps = K4A_FRAMES_PER_SECOND_30;
	config.color_format = K4A_IMAGE_FORMAT_COLOR_MJPG;
	config.color_resolution = K4A_COLOR_RESOLUTION_2160P;
	config.depth_mode = K4A_DEPTH_MODE_NFOV_UNBINNED;
	config.synchronized_images_only = false;

	if (K4A_RESULT_SUCCEEDED != k4a_device_start_cameras(device, &config))
	{
		printf("Failed to start device\n");
		//goto Exit;
	}
	k4a_capture_t capture = NULL;
	k4a_capture_create(&capture);

	// Capture a depth frame
	switch (k4a_device_get_capture(device, &capture, 1000))
	{
	case K4A_WAIT_RESULT_SUCCEEDED:
		break;
	case K4A_WAIT_RESULT_TIMEOUT:
		printf("Timed out waiting for a capture\n");
		//continue;
		break;
	case K4A_WAIT_RESULT_FAILED:
		printf("Failed to read a capture\n");
		//goto Exit;
	}

	// Application logic
	// Access the depth16 image
	k4a_image_t image = k4a_capture_get_depth_image(capture);
	if (image != NULL)
	{
		printf(" | Depth16 res:%4dx%4d stride:%5d\n",
			k4a_image_get_height_pixels(image),
			k4a_image_get_width_pixels(image),
			k4a_image_get_stride_bytes(image));

		// Release the image
		k4a_image_release(image);
	}

	// Release the capture
	k4a_capture_release(capture);

	// Shut down the camera when finished with application logic
	k4a_device_stop_cameras(device);
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
