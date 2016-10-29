/*
* Author List: L.Goutam
* Filename: MenuSelection.java
* Theme: Auto-Serving Bot - eYRC Specific
* Functions: In-Build Functions - doInBackground(JSONObject), onPostExecution(String), onPreExecute()
* Global Variables: NONE
*/
package com.autobot.foodbuddy;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.ArrayAdapter;
import android.widget.Spinner;

import org.apache.http.client.ResponseHandler;
import org.apache.http.entity.StringEntity;
import org.apache.http.impl.client.BasicResponseHandler;
import org.apache.http.params.HttpConnectionParams;
import org.json.JSONArray;
import org.json.JSONException;
import org.json.JSONObject;

import android.os.AsyncTask;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import org.apache.http.client.HttpClient;
import org.apache.http.client.methods.HttpPost;
import org.apache.http.impl.client.DefaultHttpClient;


public class MenuSelection extends AppCompatActivity {

    String table_no;
    private String[] arraySpinner;
    public String serverIP;

    Button btnPlaceOrder;
    TextView item;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_menu_selection);

        if(getIntent() != null && getIntent().getExtras() != null){
            serverIP = getIntent().getExtras().getString("server_ip_input");
        }

        // Create Spinner Values
        this.arraySpinner = new String[]{ "select table","1", "2", "3", "4", "5", "6", "7", "8", "9", "10", "11", "12", "13", "14", "15", "16" };
        final Spinner s = (Spinner) findViewById(R.id.TableNumberSpinner);
        ArrayAdapter<String> adapter = new ArrayAdapter<String>(this,
                android.R.layout.simple_spinner_item, arraySpinner);
        s.setAdapter(adapter);

        item = (TextView) findViewById(R.id.i1);


        btnPlaceOrder = (Button) findViewById(R.id.placeOrderButton);

        btnPlaceOrder.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View v)
            {
                String msg1 = "Toast created success";
                /*Toast.makeText(getBaseContext(),msg1  , Toast.LENGTH_SHORT ).show();*/
                table_no = s.getSelectedItem().toString();

                // if table_no is not selected
                if(table_no == "select table"){
                    Toast.makeText(getBaseContext(),"plese select your TABLE NUMBER", Toast.LENGTH_SHORT ).show();
                }
                else {
                    try {
                        JSONObject glob = new JSONObject();
                        JSONArray joa = new JSONArray(); // global JSONArray
                        JSONObject tab = new JSONObject();

                        tab.put("tableNo", table_no);
                        joa.put(tab);

                        // Create and Array for quantity values
                        JSONArray qtyArr = new JSONArray();

                        // Extract edittext values for quantities and append them to the Array
                        EditText e1 = (EditText) findViewById(R.id.q1);
                        qtyArr.put(e1.getText().toString());

                        EditText e2 = (EditText) findViewById(R.id.q4);
                        qtyArr.put(e2.getText().toString());

                        EditText e3 = (EditText) findViewById(R.id.q12);
                        qtyArr.put(e3.getText().toString());

                        EditText e4 = (EditText) findViewById(R.id.q13);
                        qtyArr.put(e4.getText().toString());

                        EditText e5 = (EditText) findViewById(R.id.q14);
                        qtyArr.put(e5.getText().toString());

                        EditText e6 = (EditText) findViewById(R.id.q15);
                        qtyArr.put(e6.getText().toString());

                        EditText e7 = (EditText) findViewById(R.id.q16);
                        qtyArr.put(e7.getText().toString());

                        EditText e8 = (EditText) findViewById(R.id.q17);
                        qtyArr.put(e8.getText().toString());

                        EditText e9 = (EditText) findViewById(R.id.q18);
                        qtyArr.put(e9.getText().toString());

                        EditText e10 = (EditText) findViewById(R.id.q19);
                        qtyArr.put(e10.getText().toString());

                        EditText e11 = (EditText) findViewById(R.id.q20);
                        qtyArr.put(e11.getText().toString());

                        EditText e12 = (EditText) findViewById(R.id.q21);
                        qtyArr.put(e12.getText().toString());

                        EditText e13 = (EditText) findViewById(R.id.q22);
                        qtyArr.put(e13.getText().toString());

                        // add Quantity Array to JSONObject
                        JSONObject qn = new JSONObject();
                        qn.put("qty", qtyArr);
                        joa.put(qn);

                        // Create and Array for item IDs
                        JSONArray pidArr = new JSONArray();

                        // Add all itemIDs to pidArr
                        pidArr.put("i1");
                        pidArr.put("i4");
                        pidArr.put("i12");
                        pidArr.put("i13");
                        pidArr.put("i14");
                        pidArr.put("i15");
                        pidArr.put("i16");
                        pidArr.put("i17");
                        pidArr.put("i18");
                        pidArr.put("i19");
                        pidArr.put("i20");
                        pidArr.put("i21");
                        pidArr.put("i22");

                        JSONObject id = new JSONObject();
                        id.put("pid", pidArr);
                        joa.put(id);
                        glob.put("json", joa);
                        AsyncT asyncT = new AsyncT();

                        // execute the server communication in another class so that the main thread does not block in case of connection error
                        asyncT.execute(new JSONObject[]{glob});



                        /*Print JSONObject in toast message*/
                        /*Toast.makeText(getBaseContext(),jo.toString()  , Toast.LENGTH_LONG ).show();*/
                    } catch (JSONException e) {
                        e.printStackTrace();
                    }
                }   // end of ELSE after table_no check

            }   // end of onClick
        }); // end of setOnClickListener function

    }   // end of onCreate function

    /* Inner class to get response */
    class AsyncT extends AsyncTask<JSONObject, Void, String> {

        String response;
        String php_url = "http://"+serverIP+"/android_app/take_from_android.php";

        @Override
        protected String doInBackground(JSONObject... data) {
            JSONObject jobj= data[0];
            HttpClient client = new DefaultHttpClient();
            HttpConnectionParams.setConnectionTimeout(client.getParams(), 100000);
            HttpPost httppost = new HttpPost(php_url);


            try {

                httppost.addHeader("content-type", "application/x-www-form-urlencoded");
                StringEntity se = new StringEntity(jobj.toString());
                httppost.setEntity(se);

                // Execute request and Handle response
                ResponseHandler<String> responseHandler = new BasicResponseHandler();
                response = client.execute(httppost, responseHandler);

            } catch (Exception e) {
                e.printStackTrace();
            }
            return response;
        }

        /*
        * Function Name: onPostExecute
        * Input: String
        * Output: Displays the response from server as a toast message after executing the doInBackground function
        * Example Call: cannot call this function.
        */
        @Override
        protected void onPostExecute(String response) {
            super.onPostExecute(response);
            Toast.makeText(getApplicationContext(), response, Toast.LENGTH_LONG).show();

        }

        /*
        * Function Name: go_to_MenuSelection_Activity
        * Input: NONE
        * Output: Displays a toast message before executing the doInBackground function
        * Example Call: cannot call this function.
        */
        @Override
        protected void onPreExecute() {
            super.onPreExecute();
            Toast.makeText(getApplicationContext(),"Thank You.", Toast.LENGTH_SHORT).show();
        }
    }

}



