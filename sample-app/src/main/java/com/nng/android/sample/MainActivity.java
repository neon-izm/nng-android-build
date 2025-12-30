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
 * Features self-testing with local send/receive on single device
 */
public class MainActivity extends AppCompatActivity {
    private static final String TAG = "NngSample";
    private static final String LOCAL_URL = "inproc://selftest";
    
    private NngWrapper serverSocket;
    private NngWrapper clientSocket;
    private ExecutorService executorService;
    private Handler mainHandler;
    private boolean isLocalTestActive = false;
    
    private EditText urlEditText;
    private EditText messageEditText;
    private TextView statusTextView;
    private TextView receivedTextView;
    private Button setupLocalTestButton;
    private Button listenButton;
    private Button dialButton;
    private Button sendButton;
    private Button receiveButton;
    private Button sendToSelfButton;
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
        setupLocalTestButton = findViewById(R.id.setupLocalTestButton);
        listenButton = findViewById(R.id.listenButton);
        dialButton = findViewById(R.id.dialButton);
        sendButton = findViewById(R.id.sendButton);
        receiveButton = findViewById(R.id.receiveButton);
        sendToSelfButton = findViewById(R.id.sendToSelfButton);
        closeButton = findViewById(R.id.closeButton);
        
        // Set default values
        urlEditText.setText(LOCAL_URL);
        messageEditText.setText("Hello from NNG Android!");
        
        // Set click listeners
        setupLocalTestButton.setOnClickListener(this::onSetupLocalTestClick);
        listenButton.setOnClickListener(this::onListenClick);
        dialButton.setOnClickListener(this::onDialClick);
        sendButton.setOnClickListener(this::onSendClick);
        receiveButton.setOnClickListener(this::onReceiveClick);
        sendToSelfButton.setOnClickListener(this::onSendToSelfClick);
        closeButton.setOnClickListener(this::onCloseClick);
        
        // Initial button states
        enableButtons(false, false, false, false, false, false);
        setupLocalTestButton.setEnabled(true);
    }
    
    private void initNng() {
        try {
            executorService = Executors.newFixedThreadPool(2); // For server and client
            mainHandler = new Handler(Looper.getMainLooper());
            
            updateStatus("NNG ready - Click 'Setup Local Test' to begin");
            
            // Test NNG version with temporary wrapper
            NngWrapper tempWrapper = new NngWrapper();
            Log.i(TAG, "NNG Version: " + tempWrapper.nngVersion());
            
        } catch (Exception e) {
            Log.e(TAG, "Failed to initialize NNG", e);
            updateStatus("Failed to initialize NNG: " + e.getMessage());
        }
    }
    
    private void onSetupLocalTestClick(View view) {
        updateStatus("Setting up local test environment...");
        
        executorService.execute(() -> {
            try {
                // Create server socket
                serverSocket = new NngWrapper();
                serverSocket.openPair0();
                serverSocket.listen(LOCAL_URL);
                Log.i(TAG, "Server socket listening on: " + LOCAL_URL);
                
                // Small delay to ensure server is ready
                Thread.sleep(100);
                
                // Create client socket
                clientSocket = new NngWrapper();
                clientSocket.openPair0();
                clientSocket.dial(LOCAL_URL);
                Log.i(TAG, "Client socket connected to: " + LOCAL_URL);
                
                // Start auto-receive thread for server
                startAutoReceive();
                
                isLocalTestActive = true;
                
                mainHandler.post(() -> {
                    updateStatus("Local test ready! Server and client connected.");
                    enableButtons(true, true, true, true, true, false);
                    setupLocalTestButton.setEnabled(false);
                    Toast.makeText(MainActivity.this, "Local test setup complete!", Toast.LENGTH_SHORT).show();
                });
                
            } catch (Exception e) {
                Log.e(TAG, "Failed to setup local test", e);
                mainHandler.post(() -> {
                    updateStatus("Setup failed: " + e.getMessage());
                    Toast.makeText(MainActivity.this, "Setup failed: " + e.getMessage(), Toast.LENGTH_LONG).show();
                });
            }
        });
    }
    
    private void startAutoReceive() {
        executorService.execute(() -> {
            while (isLocalTestActive && serverSocket != null) {
                try {
                    String receivedMessage = serverSocket.receiveString();
                    
                    mainHandler.post(() -> {
                        String currentText = receivedTextView.getText().toString();
                        String newText = currentText + "\n[" + getCurrentTime() + "] Received: " + receivedMessage;
                        receivedTextView.setText(newText);
                        updateStatus("Auto-received message: " + receivedMessage);
                    });
                    
                } catch (Exception e) {
                    if (isLocalTestActive) {
                        Log.e(TAG, "Auto-receive error", e);
                    }
                    break;
                }
            }
        });
    }
    
    private String getCurrentTime() {
        return new java.text.SimpleDateFormat("HH:mm:ss", java.util.Locale.getDefault())
                .format(new java.util.Date());
    }
    
    private void onSendToSelfClick(View view) {
        String message = messageEditText.getText().toString();
        if (message.isEmpty()) {
            Toast.makeText(this, "Please enter a message", Toast.LENGTH_SHORT).show();
            return;
        }
        
        if (!isLocalTestActive || clientSocket == null) {
            Toast.makeText(this, "Local test not active", Toast.LENGTH_SHORT).show();
            return;
        }
        
        executorService.execute(() -> {
            try {
                clientSocket.sendString(message);
                
                mainHandler.post(() -> {
                    updateStatus("Sent to self: " + message);
                    Toast.makeText(MainActivity.this, "Message sent to self!", Toast.LENGTH_SHORT).show();
                });
                
            } catch (Exception e) {
                Log.e(TAG, "Failed to send to self", e);
                mainHandler.post(() -> {
                    updateStatus("Send to self failed: " + e.getMessage());
                    Toast.makeText(MainActivity.this, "Send failed: " + e.getMessage(), Toast.LENGTH_LONG).show();
                });
            }
        });
    }

    private void onListenClick(View view) {
        String url = urlEditText.getText().toString().trim();
        if (url.isEmpty()) {
            Toast.makeText(this, "Please enter a URL", Toast.LENGTH_SHORT).show();
            return;
        }
        
        executorService.execute(() -> {
            try {
                if (serverSocket == null) {
                    serverSocket = new NngWrapper();
                }
                serverSocket.openPair0();
                serverSocket.listen(url);
                
                mainHandler.post(() -> {
                    updateStatus("Listening on: " + url);
                    enableButtons(false, false, true, true, false, true);
                });
                
            } catch (Exception e) {
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
                if (clientSocket == null) {
                    clientSocket = new NngWrapper();
                }
                clientSocket.openPair0();
                clientSocket.dial(url);
                
                mainHandler.post(() -> {
                    updateStatus("Connected to: " + url);
                    enableButtons(false, false, true, true, false, true);
                });
                
            } catch (Exception e) {
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
                NngWrapper activeSocket = isLocalTestActive ? clientSocket : serverSocket;
                if (activeSocket != null) {
                    activeSocket.sendString(message);
                    
                    mainHandler.post(() -> {
                        updateStatus("Message sent: " + message);
                        Toast.makeText(MainActivity.this, "Message sent successfully", Toast.LENGTH_SHORT).show();
                    });
                } else {
                    mainHandler.post(() -> {
                        updateStatus("No active socket for sending");
                        Toast.makeText(MainActivity.this, "No active socket", Toast.LENGTH_SHORT).show();
                    });
                }
                
            } catch (Exception e) {
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
                NngWrapper activeSocket = isLocalTestActive ? serverSocket : clientSocket;
                if (activeSocket != null) {
                    String receivedMessage = activeSocket.receiveString();
                    
                    mainHandler.post(() -> {
                        updateStatus("Manual receive: " + receivedMessage);
                        String currentText = receivedTextView.getText().toString();
                        String newText = currentText + "\n[" + getCurrentTime() + "] Manual: " + receivedMessage;
                        receivedTextView.setText(newText);
                        Toast.makeText(MainActivity.this, "Message received", Toast.LENGTH_SHORT).show();
                    });
                } else {
                    mainHandler.post(() -> {
                        updateStatus("No active socket for receiving");
                        Toast.makeText(MainActivity.this, "No active socket", Toast.LENGTH_SHORT).show();
                    });
                }
                
            } catch (Exception e) {
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
                isLocalTestActive = false;
                
                if (serverSocket != null) {
                    serverSocket.close();
                    serverSocket = null;
                }
                
                if (clientSocket != null) {
                    clientSocket.close();
                    clientSocket = null;
                }
                
                mainHandler.post(() -> {
                    updateStatus("All sockets closed");
                    receivedTextView.setText("");
                    enableButtons(false, false, false, false, false, false);
                    setupLocalTestButton.setEnabled(true);
                    Toast.makeText(MainActivity.this, "All sockets closed", Toast.LENGTH_SHORT).show();
                });
                
            } catch (Exception e) {
                Log.e(TAG, "Error closing sockets", e);
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
    
    private void enableButtons(boolean listen, boolean dial, boolean send, boolean receive, boolean sendToSelf, boolean close) {
        listenButton.setEnabled(listen);
        dialButton.setEnabled(dial);
        sendButton.setEnabled(send);
        receiveButton.setEnabled(receive);
        sendToSelfButton.setEnabled(sendToSelf);
        closeButton.setEnabled(close);
    }
    
    @Override
    protected void onDestroy() {
        super.onDestroy();
        
        isLocalTestActive = false;
        
        if (executorService != null) {
            executorService.shutdown();
        }
        
        if (serverSocket != null) {
            try {
                serverSocket.close();
            } catch (Exception e) {
                Log.e(TAG, "Error closing server socket in onDestroy", e);
            }
        }
        
        if (clientSocket != null) {
            try {
                clientSocket.close();
            } catch (Exception e) {
                Log.e(TAG, "Error closing client socket in onDestroy", e);
            }
        }
    }
}
