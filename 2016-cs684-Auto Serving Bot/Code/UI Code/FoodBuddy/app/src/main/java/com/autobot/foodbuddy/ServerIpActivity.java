/*
* Author List: L.Goutam
* Filename: ServerIpActivity.java
* Theme: Auto-Serving Bot - eYRC Specific
* Functions: go_to_homescreen()
* Global Variables: NONE
*/
package com.autobot.foodbuddy;

import android.content.Intent;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import org.apache.http.conn.util.InetAddressUtils;

public class ServerIpActivity extends AppCompatActivity {

    String ip_string;
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_server_ip);
        go_to_homescreen();
    }

    Button btnIpSubmit;
    /*
    * Function Name: go_to_homescreen
    * Input: NONE
    * Output: Transfers the user from "ServerIp Input" Screen to Main Screen
    * Logic: When the user clicks the btnIpSubmit button, the new activity is opened
    * Example Call: go_to_MenuSelection_Activity();
    */
    private void go_to_homescreen(){
        btnIpSubmit = (Button)findViewById(R.id.btnIpSubmit);
        btnIpSubmit.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                EditText e1 = (EditText) findViewById(R.id.serverIpED);
                ip_string = e1.getText().toString();

                // check the IP address format
                if( InetAddressUtils.isIPv4Address(ip_string)) {
                    Intent i = new Intent(ServerIpActivity.this, HomeScreen.class);
                    i.putExtra("server_ip_input", ip_string);   // pass the serverIP to the next Activity screen
                    startActivity(i);
                }
                else{   // if format is not in IP4 Address format, toast a msg
                    Toast.makeText(getBaseContext(),"Please input a valid IP address"  , Toast.LENGTH_SHORT ).show();
                }

            }
        });
    }
}
