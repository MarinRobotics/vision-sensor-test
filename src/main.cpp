#include "main.h"
#include <string>
#include <sys/types.h>


//import string type
using std::string;


static uint16_t consoleLine = 0;
/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */
void on_center_button() {
}

void printLine(string data){
	if (pros::lcd::is_initialized()){
		pros::lcd::set_text(consoleLine++, data);
	}
	else{ //if LCD is not initialized:
		pros::lcd::initialize();
		pros::lcd::set_text(consoleLine++, data );
	}
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	
	//printing init
	printLine("PROS LCD is initialized");

	pros::lcd::register_btn1_cb(on_center_button);
}

void clearText(){
	consoleLine = 0;
	pros::lcd::clear();
}
/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {}




/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {
	pros::Controller master(pros::E_CONTROLLER_MASTER);
	//init vision sensor: 
	pros::Vision visionSensor (1);

	pros::delay(1000);

	//blue and red signature definition
	pros::vision_signature_s_t BLUE_GOAL = pros::Vision::signature_from_utility(1, -2643, -2227, -2435, 12859, 14783, 13821, 11.000, 0);
	pros::vision_signature_s_t RED_GOAL = pros::Vision::signature_from_utility(2, 4481, 7595, 6038, 2621, 4661, 3641, 3.000, 0);

	//set signatures to corrosponding id's:
	visionSensor.set_signature(1, &BLUE_GOAL);
	visionSensor.set_signature(2, &RED_GOAL);

	//create arrays for the objects
	pros::vision_object_s_t blueArr[10];
	pros::vision_object_s_t redArr[10];

	while (true) {
		//get largest blue object by signature:
		visionSensor.read_by_sig(0,1,10,blueArr);
		pros::vision_object_s_t largestBlue = blueArr[0];
		//generate blue data string:
		string blueData = "BLUE w: " + std::to_string(largestBlue.width) + "; h: " + std::to_string(largestBlue.height) + "; s: " + std::to_string(largestBlue.signature);
		//print data:
		printLine(blueData);






		//get largest red object by signature:
		visionSensor.read_by_sig(0,2,10,redArr);
		pros::vision_object_s_t largestRed = redArr[0];
		//generate red data string:
		string redData = "RED w: " + std::to_string(largestRed.width) + "; h: " + std::to_string(largestRed.height) + "; s: " + std::to_string(largestRed.signature);
		//print data:
		printLine(redData);

		pros::delay(10);

		clearText();
	}
}
