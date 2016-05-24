package com.bq.robotic.robopad;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.view.View;

import com.bq.robotic.robopad.utils.RoboPadConstants;


public class SelectRobotActivity extends Activity {

    // Debugging
    private static final String LOG_TAG = "SelectRobotActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_select_robot);
    }

    /**
     * Callback for the clicks on the robot buttons to select one of the robot types
     * @param v the button clicked
     */
    public void onRobotSelected(View v) {

        Intent intent = new Intent(SelectRobotActivity.this, RoboPad.class);

        switch(v.getId()) {

            case R.id.pollywog_button:
                intent.putExtra(RoboPadConstants.ROBOT_SELECTED_KEY, RoboPadConstants.robotType.POLLYWOG);
                break;

            case R.id.beetle_button:
                intent.putExtra(RoboPadConstants.ROBOT_SELECTED_KEY, RoboPadConstants.robotType.BEETLE);
                break;

            case R.id.evolution_button:
                intent.putExtra(RoboPadConstants.ROBOT_SELECTED_KEY, RoboPadConstants.robotType.EVOLUTION);
                break;

            case R.id.rhino_button:
                intent.putExtra(RoboPadConstants.ROBOT_SELECTED_KEY, RoboPadConstants.robotType.RHINO);
                break;

            case R.id.crab_button:
                intent.putExtra(RoboPadConstants.ROBOT_SELECTED_KEY, RoboPadConstants.robotType.CRAB);
                break;

            case R.id.generic_button:
                intent.putExtra(RoboPadConstants.ROBOT_SELECTED_KEY, RoboPadConstants.robotType.GENERIC_ROBOT);
                break;

        }

        startActivity(intent);

    }


    /**
     * Callback for the show preferences button
     * @param v
     */
    public void onShowPreferences(View v) {
        Intent intent = new Intent(SelectRobotActivity.this,
                RoboPadSettings.class);
        startActivity(intent);
    }

}
