// import general java stuff for server
import java.io.BufferedReader;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.UnknownHostException;
import java.util.Set;
import java.lang.reflect.Method;

// import java-websocket stuff
import org.java_websocket.WebSocket;
import org.java_websocket.handshake.ClientHandshake;
import org.java_websocket.server.WebSocketServer;

class SimpleWSServer extends WebSocketServer {
        PApplet parent;
        Method  onMessageMethod;
  
        public SimpleWSServer( PApplet app, int port ) throws UnknownHostException {
		super( new InetSocketAddress( port ) );
		WebSocket.DEBUG = true;
                parent = app;
                
                try {
                  onMessageMethod = parent.getClass().getMethod("onMessage", new Class[]{String.class});
                } catch (Exception e){
                  println("beef");
                }
                
                start();
	}

	@Override
	public void onOpen( WebSocket conn, ClientHandshake handshake ) {
		//do something in the future?
                // parent.onOpen( WebSocket conn );
	}

	@Override
	public void onClose( WebSocket conn, int code, String reason, boolean remote ) {
		//do something in the future?
                // parent.onOpen( conn, code, reason, remote );
	}

	@Override
	public void onMessage( WebSocket conn, String message ) {
                if ( onMessageMethod != null ){
                  try {
                    onMessageMethod.invoke( parent, message);
                  } catch( Exception e ){
                    println("onMessage invoke failed, disabling :(");
                    onMessageMethod = null;
                  }
                }
	}

	@Override
	public void onError( WebSocket conn, Exception ex ) {
		ex.printStackTrace();
	}

	/**
	 * Sends <var>text</var> to all currently connected WebSocket clients.
	 * 
	 * @param text
	 *            The String to send across the network.
	 * @throws InterruptedException
	 *             When socket related I/O errors occur.
	 */
	public void sendToAll( String text ) {
		Set<WebSocket> con = connections();
		synchronized ( con ) {
			for( WebSocket c : con ) {
				c.send( text );
			}
		}
	}
}
