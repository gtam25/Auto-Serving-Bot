/*
* Author List: L.Goutam
* Filename: HomeScreen.java
* Theme: Auto-Serving Bot - eYRC Specific
* Functions: onCreate(Bundle ), go_to_MenuSelection_Activity(), go_to_ContactDetails_Activity(), go_to_About_Activity()
* Global Variables: NONE
*/

package com.autobot.foodbuddy;

import android.net.Uri;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.content.Intent;

import com.google.android.gms.appindexing.Action;
import com.google.android.gms.appindexing.AppIndex;
import com.google.android.gms.appindexing.Thing;
import com.google.android.gms.common.api.GoogleApiClient;

public class HomeScreen extends AppCompatActivity {

    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    private GoogleApiClient client;
    private String serverIP;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home_screen);

        // get the serverIP from previous screen to pass it to next screen
        if(getIntent() != null && getIntent().getExtras() != null){
            serverIP = getIntent().getExtras().getString("server_ip_input");
        }

        go_to_MenuSelection_Activity();
        go_to_ContactDetails_Activity();
        go_to_About_Activity();
        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client = new GoogleApiClient.Builder(this).addApi(AppIndex.API).build();
    }

    /*
    * Function Name: go_to_MenuSelection_Activity
    * Input: NONE
    * Output: Transfers the user from Main screen to MenuSelection Screen
    * Logic: When the user clicks the btnGoToMenuSelection button, the new activity is opened
    * Example Call: go_to_MenuSelection_Activity();
    */
    private Button btnGoToMenuSelection;
    private void go_to_MenuSelection_Activity() {
        btnGoToMenuSelection = (Button)findViewById(R.id.MenuButton);
        btnGoToMenuSelection.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                Intent i = new Intent(HomeScreen.this, MenuSelection.class);
                i.putExtra("server_ip_input", serverIP);    // pass the serverIP to the next Activity screen
                startActivity(i);
            }
        }
        );
    }

    /*
    * Function Name: go_to_ContactDetails_Activity
    * Input: NONE
    * Output: Transfers the user from Main screen to Contact Info Screen
    * Logic: When the user clicks the btnGoToContactDetails button, the new activity is opened
    * Example Call: go_to_ContactDetails_Activity();
    */
    private Button btnGoToContactDetails;
    private void go_to_ContactDetails_Activity() {
        btnGoToContactDetails = (Button)findViewById(R.id.ContactButton);
        btnGoToContactDetails.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                Intent j = new Intent(HomeScreen.this, ContactDetails.class);

                startActivity(j);
            }
        }
        );
    }

    /*
    * Function Name: go_to_About_Activity
    * Input: NONE
    * Output: Transfers the user from Main screen to About Screen
    * Logic: When the user clicks the btnGoToAbout button, the new activity is opened
    * Example Call: go_to_About_Activity();
    */
    private Button btnGoToAbout;
    private void go_to_About_Activity() {
        btnGoToAbout = (Button)findViewById(R.id.AboutButton);
        btnGoToAbout.setOnClickListener(new View.OnClickListener(){
            @Override
            public void onClick(View v) {
                Intent k = new Intent(HomeScreen.this, About.class);
                startActivity(k);
            }
            }
        );
    }



    /**
     * ATTENTION: This was auto-generated to implement the App Indexing API.
     * See https://g.co/AppIndexing/AndroidStudio for more information.
     */
    public Action getIndexApiAction() {
        Thing object = new Thing.Builder()
                .setName("HomeScreen Page") // TODO: Define a title for the content shown.
                // TODO: Make sure this auto-generated URL is correct.
                .setUrl(Uri.parse("http://[ENTER-YOUR-URL-HERE]"))
                .build();
        return new Action.Builder(Action.TYPE_VIEW)
                .setObject(object)
                .setActionStatus(Action.STATUS_TYPE_COMPLETED)
                .build();
    }

    @Override
    public void onStart() {
        super.onStart();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        client.connect();
        AppIndex.AppIndexApi.start(client, getIndexApiAction());
    }

    @Override
    public void onStop() {
        super.onStop();

        // ATTENTION: This was auto-generated to implement the App Indexing API.
        // See https://g.co/AppIndexing/AndroidStudio for more information.
        AppIndex.AppIndexApi.end(client, getIndexApiAction());
        client.disconnect();
    }
}
