package com.nng.android.sample;

import android.os.Bundle;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.app.AppCompatActivity;

import com.nng.android.NngWrapper;

import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

/**
 * Sample application demonstrating NNG Android Library usage
 */
public class MainActivity extends AppCompatActivity {
    private static final String TAG = "NngSample";
    
    private NngWrapper nngWrapper;
    private ExecutorService executorService;
    private Handler mainHandler;
    
    private EditText urlEditText;
    private EditText messageEditText;
    private TextView statusTextView;
    private TextView receivedTextView;
    private Button listenButton;
    private Button dialButton;
    private Button sendButton;
    private Button receiveButton;
    private Button closeButton;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        
        initViews();
        initNng();
    }
    
    private void initViews() {
        urlEditText = findViewById(R.id.urlEditText);
        messageEditText = findViewById(R.id.messageEditText);
        statusTextView = findViewById(R.id.statusTextView);
        receivedTextView = findViewById(R.id.receivedTextView);
        listenButton = findViewById(R.id.listenButton);
        dialButton = findViewById(R.id.dialButton);
        sendButton = findViewById(R.id.sendButton);
        receiveButton = findViewById(R.id.receiveButton);
        closeButton = findViewById(R.id.closeButton);
        
        // Set default values
        urlEditText.setText("inproc://test");
        messageEditText.setText("Hello from NNG Android!");
        
        // Set click listeners
        listenButton.setOnClickListener(this::onListenClick);
        dialButton.setOnClickListener(this::onDialClick);
        sendButton.setOnClickListener(this::onSendClick);
        receiveButton.setOnClickListener(this::onReceiveClick);
        closeButton.setOnClickListener(this::onCloseClick);
    }
    
    private void initNng() {
        try {
            nngWrapper = new NngWrapper();
            executorService = Executors.newSingleThreadExecutor();
            mainHandler = new Handler(Looper.getMainLooper());
            
            updateStatus("NNG initialized successfully");
            Log.i(TAG, "NNG Version: " + nngWrapper.nngVersion());
            
        } catch (Exception e) {
            Log.e(TAG, "Failed to initialize NNG", e);
            updateStatus("Failed to initialize NNG: " + e.getMessage());
        }
    }
    
    private void onListenClick(View view) {
        String url = urlEditText.getText().toString().trim();
        if (url.isEmpty()) {
            Toast.makeText(this, "Please enter a URL", Toast.LENGTH_SHORT).show();
            return;
        }
        
        executorService.execute(() -> {
            try {
                nngWrapper.openPair0();
                nngWrapper.listen(url);
                
                mainHandler.post(() -> {
                    updateStatus("Listening on: " + url);
                    enableButtons(false, false, true, true, true);
                });
                
            } catch (NngWrapper.NngException e) {
                Log.e(TAG, "Failed to listen", e);
                mainHandler.post(() -> {
                    updateStatus("Listen failed: " + e.getMessage());
                    Toast.makeText(MainActivity.this, "Listen failed: " + e.getMessage(), Toast.LENGTH_LONG).show();
                });
            }
        });
    }
    
    private void onDialClick(View view) {
        String url = urlEditText.getText().toString().trim();
        if (url.isEmpty()) {
            Toast.makeText(this, "Please enter a URL", Toast.LENGTH_SHORT).show();
            return;
        }
        
        executorService.execute(() -> {
            try {
                nngWrapper.openPair0();
                nngWrapper.dial(url);
                
                mainHandler.post(() -> {
                    updateStatus("Connected to: " + url);
                    enableButtons(false, false, true, true, true);
                });
                
            } catch (NngWrapper.NngException e) {
                Log.e(TAG, "Failed to dial", e);
                mainHandler.post(() -> {
                    updateStatus("Dial failed: " + e.getMessage());
                    Toast.makeText(MainActivity.this, "Dial failed: " + e.getMessage(), Toast.LENGTH_LONG).show();
                });
            }
        });
    }
    
    private void onSendClick(View view) {
        String message = messageEditText.getText().toString();
        if (message.isEmpty()) {
            Toast.makeText(this, "Please enter a message", Toast.LENGTH_SHORT).show();
            return;
        }
        
        executorService.execute(() -> {
            try {
                nngWrapper.sendString(message);
                
                mainHandler.post(() -> {
                    updateStatus("Message sent: " + message);
                    Toast.makeText(MainActivity.this, "Message sent successfully", Toast.LENGTH_SHORT).show();
                });
                
            } catch (NngWrapper.NngException e) {
                Log.e(TAG, "Failed to send", e);
                mainHandler.post(() -> {
                    updateStatus("Send failed: " + e.getMessage());
                    Toast.makeText(MainActivity.this, "Send failed: " + e.getMessage(), Toast.LENGTH_LONG).show();
                });
            }
        });
    }
    
    private void onReceiveClick(View view) {
        executorService.execute(() -> {
            try {
                String receivedMessage = nngWrapper.receiveString();
                
                mainHandler.post(() -> {
                    updateStatus("Message received");
                    receivedTextView.setText("Received: " + receivedMessage);
                    Toast.makeText(MainActivity.this, "Message received", Toast.LENGTH_SHORT).show();
                });
                
            } catch (NngWrapper.NngException e) {
                Log.e(TAG, "Failed to receive", e);
                mainHandler.post(() -> {
                    updateStatus("Receive failed: " + e.getMessage());
                    Toast.makeText(MainActivity.this, "Receive failed: " + e.getMessage(), Toast.LENGTH_LONG).show();
                });
            }
        });
    }
    
    private void onCloseClick(View view) {
        executorService.execute(() -> {
            try {
                nngWrapper.close();
                
                mainHandler.post(() -> {
                    updateStatus("Socket closed");
                    receivedTextView.setText("");
                    enableButtons(true, true, false, false, false);
                    Toast.makeText(MainActivity.this, "Socket closed", Toast.LENGTH_SHORT).show();
                });
                
            } catch (Exception e) {
                Log.e(TAG, "Error closing socket", e);
                mainHandler.post(() -> {
                    updateStatus("Close error: " + e.getMessage());
                });
            }
        });
    }
    
    private void updateStatus(String status) {
        statusTextView.setText("Status: " + status);
        Log.i(TAG, status);
    }
    
    private void enableButtons(boolean listen, boolean dial, boolean send, boolean receive, boolean close) {
        listenButton.setEnabled(listen);
        dialButton.setEnabled(dial);
        sendButton.setEnabled(send);
        receiveButton.setEnabled(receive);
        closeButton.setEnabled(close);
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        
        if (executorService != null) {
            executorService.shutdown();
        }
        
        if (nngWrapper != null) {
            try {
                nngWrapper.close();
            } catch (Exception e) {
                Log.e(TAG, "Error closing NNG in onDestroy", e);
            }
        }
    }
}
