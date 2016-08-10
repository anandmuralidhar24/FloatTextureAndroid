/*
 *    Copyright 2016 Anand Muralidhar
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

package com.anandmuralidhar.floattextureandroid;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;
import android.os.AsyncTask;
import android.os.Bundle;


public class FloatTextureActivity extends Activity{

    private GLSurfaceView mGLView = null;
    private boolean appIsExiting=false;

    private native void CreateObjectNative(AssetManager assetManager, String pathToInternalDir);
    private native void DeleteObjectNative();
    private native boolean IsInitsDoneNative();
    private native int GetGLESVersionNative();

    private Activity mActivity;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        AssetManager assetManager = getAssets();
        String pathToInternalDir = getFilesDir().getAbsolutePath();

        // call the native constructors to create an object
        CreateObjectNative(assetManager, pathToInternalDir);

        // layout has only two components, a GLSurfaceView and a TextView
        setContentView(R.layout.floattexture_layout);
        mGLView = (MyGLSurfaceView) findViewById (R.id.gl_surface_view);

        mActivity = this;
    }

    @Override
    protected void onResume() {

        super.onResume();

        if(appIsExiting) {
            return;
        }

        // Android suggests that we call onResume on GLSurfaceView
        if (mGLView != null) {
            mGLView.onResume();
        }

        new AsyncCheckGLESVersion().execute();

    }

    @Override
    protected void onPause() {

        super.onPause();

        // Android suggests that we call onPause on GLSurfaceView
        if(mGLView != null) {
            mGLView.onPause();
        }
    }

    @Override
    protected void onDestroy() {

        super.onDestroy();

        // We are exiting the activity, let's delete the native objects
        DeleteObjectNative();

    }

    public void ShowExitDialog(final Activity activity, String exitMessage){
        appIsExiting = true;
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(activity);
        alertDialogBuilder.setMessage(exitMessage)
                .setCancelable(false)
                .setPositiveButton("OK", new DialogInterface.OnClickListener() {
                    @Override
                    public void onClick(DialogInterface arg0, int arg1) {
                        activity.finish();
                    }
                });

        AlertDialog alertDialog = alertDialogBuilder.create();
        alertDialog.show();
    }


    // this task simply waits till all inits are completed in native code
    // then it displays the GLES version on the screen
    class AsyncCheckGLESVersion extends AsyncTask<Void, String, Integer> {

        @Override
        // this function executes on a background thread
        // hence we cannot make GL calls or manipulate UI elements here
        protected Integer doInBackground(Void... params) {

            // keep polling to check if native objects are initialized
            while(!IsInitsDoneNative()) {
                try {
                    Thread.sleep(100);
                } catch (InterruptedException e) {
                    e.printStackTrace();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
            return 0;
        }

        @Override
        // this function executes on the UI thread and we can modify UI elements
        protected void onPostExecute(Integer result) {

            int glesVersion = GetGLESVersionNative();
            if (glesVersion != 3) {
                // exit the app since OpenGL ES 3+ is not supported
                ShowExitDialog(mActivity, getString(R.string.exit_no_gles3));
            }
        }
    }

    /**
     * load libFloatTextureNative.so since it has all the native functions
     */
    static {
        System.loadLibrary("FloatTextureNative");
    }
}
