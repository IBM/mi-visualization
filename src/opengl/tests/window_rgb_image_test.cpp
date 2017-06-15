/*!
 * \file window_grayscale_image_test.cpp
 * \brief Program for testing ImageEncoder and Visualization.
 * \author tkornuta
 * \date Nov 20, 2015
 *
 * Copyright (c) 2017, Tomasz Kornuta, IBM Corporation. All rights reserved.
 *
 */

#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#include <fstream>

#include <types/MNISTTypes.hpp>
#include <types/TensorTypes.hpp>

#include <logger/Log.hpp>
#include <logger/ConsoleOutput.hpp>
using namespace mic::logger;

#include <data_io/BMPImporter.hpp>

#include <opengl/visualization/WindowManager.hpp>
#include <opengl/visualization/WindowRGBTensor.hpp>
using namespace mic::opengl::visualization;

/// Window displaying the image.
WindowRGBTensor<float>* w_batch;

/*!
 * \brief Function for testing ImageEncoder/WindowImage2D classes.
 * \author tkornuta
 */
void test_thread_body (void) {

	// Import exemplary BMPs.
	mic::data_io::BMPImporter<float> importer ("bmp_importer");
	importer.setDataFilename("../data/lena.bmp;../data/lena_eye.bmp;../data/lena_fovea.bmp;../data/rainbow.bmp");
	if (!importer.importData())
		return;

 	// Main application loop.
	while (!APP_STATE->Quit()) {

		// If not paused.
		if (!APP_STATE->isPaused()) {

			// If single step mode - pause after the step.
			if (APP_STATE->isSingleStepModeOn())
				APP_STATE->pressPause();

			{ // Enter critical section - with the use of scoped lock from AppState!
				APP_DATA_SYNCHRONIZATION_SCOPED_LOCK();

				// Select random image-label pair.
				mic::types::TensorSample<float> sample = importer.getRandomSample();

				// Set sample to be displayed.
				//w_batch->setBatchUnsynchronized(importer.data());
				w_batch->setSampleUnsynchronized(sample.data());
			}//: end of critical section

		}//: if

		// Sleep.
		APP_SLEEP();
	}//: while

}//: image_encoder_and_visualization_test



/*!
 * \brief Main program function. Runs two threads: main (for GLUT) and another one (for data processing).
 * \author tkornuta
 * @param[in] argc Number of parameters (not used).
 * @param[in] argv List of parameters (not used).
 * @return (not used)
 */
int main(int argc, char* argv[]) {
	// Set console output to logger.
	LOGGER->addOutput(new ConsoleOutput());
	LOG(LINFO) << "Logger initialized. Starting application";

	// Initialize GLUT! :]
	VGL_MANAGER->initializeGLUT(argc, argv);

	// Create two visualization windows - in the same, main thread :]
	w_batch = new WindowRGBTensor<float>("Batch", RGB::Chan_Separate, RGB::Norm_None, RGB::Grid_Batch, 0, 0, 512, 512);

	boost::thread test_thread(boost::bind(&test_thread_body));

	// Start visualization thread.
	VGL_MANAGER->startVisualizationLoop();

	LOG(LINFO) << "Waiting for threads to join...";
	// End test thread.
	test_thread.join();
	LOG(LINFO) << "Threads joined - ending application";
}//: main
