package cn.edu.sjtu.contender_klk;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

public class ContenderTest extends AppCompatActivity {

    Button mButton;
    TextView mTextView;
    int Num = 0;
    Boolean flag = false;
    Thread newThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_contender_test);
        mButton = (Button) findViewById(R.id.Start);
        mTextView = (TextView) findViewById(R.id.output);
    }
    public void Btn_Listener(View view)
    {
        flag = !flag;
        String mString;
        if (flag)
        {
            mString = "Calculating ... ";
        }
        else
        {
            mString = "Run Over";
        }
        mTextView.setText(mString);
        if(flag)
        {
            newThread = new Thread(new Runnable() {
                @Override
                public void run() {
                    while (flag)
                    {
                        Num ++;
                        Num --;
                    }
                }
            });
            newThread.start();
        }
    }
}
