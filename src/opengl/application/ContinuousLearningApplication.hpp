/*!
 * \file ContinuousLearningApplication.hpp
 * \brief 
 * \author tkornut
 * \date Jan 15, 2016
 */

#ifndef SRC_OPENGL_APPLICATION_CONTINUOUSLEARNINGAPPLICATION_HPP_
#define SRC_OPENGL_APPLICATION_CONTINUOUSLEARNINGAPPLICATION_HPP_

#include <opengl/application/ConsoleOpenGLApplication.hpp>

namespace mic {
namespace opengl {
namespace application {

class ContinuousLearningApplication : public opengl::application::ConsoleOpenGLApplication {
public:

	/*!
	 * Default constructor. Sets the application/node name and registers properties.
	 * @param node_name_ Name of the application/node (in configuration file).
	 */
	ContinuousLearningApplication(std::string node_name_);

	/*!
	 * Default destructor - empty.
	 */
	virtual ~ContinuousLearningApplication() { };

protected:

	/*!
	 * \brief Function realizing the processing thread.
	 * \author tkornuta
	 */
	virtual void processingThread(void);

	/*!
	 * Perform learning - abstract, to be overridden.
	 */
	virtual void performLearning() = 0;

	/*!
	 * Testing is divided into two phases: collection of test statistics and their population.
	 * The former is executed in every testing step, whereas the latter only every number_of_averaged_test_measures testing steps.
	 */
	virtual void performTesting();

	/*!
	 * Collects test statistics, executed in every testing step - virtual, empty now, to be overridden.
	 */
	virtual void collectTestStatistics() { };

	/*!
	 * Populates test statistics, executed only every number_of_averaged_test_measures testing steps - virtual, empty now, to be overridden.
	 */
	virtual void populateTestStatistics() { };

	/// Number of learning steps after which a single classification test will be performed.
	mic::configuration::Property<unsigned int> learning_iterations_to_test_ratio;

	/// Numbers of steps that will be averages
	mic::configuration::Property<unsigned int> number_of_averaged_test_measures;

	/// Learning iteration counter - used in interlaces learning/testing mode.
	unsigned long learning_iteration;

};

} /* namespace application */
} /* namespace opengl */
} /* namespace mic */

#endif /* SRC_OPENGL_APPLICATION_CONTINUOUSLEARNINGAPPLICATION_HPP_ */
