import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class Kliens{

	
	public static final String SERVER_ADDRESS = "127.0.0.1";
	public static final int SERVER_PORT = 25000;
	public static final char[] ELSO = { 'A', 'B', 'C', 'D', 'E','F','G','H','I','J'};
	public static final char[] MASODIK = { '0', '1', '2', '3', '4','5','6','7','8','9'};


	public static boolean Bemenet(String bemenet){
		
		//a feladás egy különleges eset ezért külön kezeljük
		if(bemenet.equals("Feladom"))
		{
			return true;
		}
	
		if(bemenet.length()!=2)
		{
			System.out.println("A bemenetnek 2 karakter hosszunak kell lennie!");
			return false;
		}
		

		for(int i=0; i<2; i++)
		{
			boolean helyese = false;
			if(i==0)
			{
				for(int j=0; j<10; j++)
				{
					if(bemenet.charAt(i)==ELSO[j])
					{
						helyese = true;
						break;
					}
				}
			}
			else{
			for(int j=0; j<10; j++)
				{
					if(bemenet.charAt(i)==MASODIK[j])
					{
						helyese = true;
						break;
					}
				}
			}
			if(helyese)
			return true;
			else return false;
			
		}
		return true;
	}




	public static void main(String[] args) throws Exception
	{
		String serverAddress = SERVER_ADDRESS;
		int serverPort = SERVER_PORT;
		Socket socket = new Socket(serverAddress, serverPort);
		BufferedReader stdIn = new BufferedReader(new InputStreamReader(System.in));
		BufferedReader socketIn = new BufferedReader(new InputStreamReader(socket.getInputStream()));
		PrintWriter socketOut = new PrintWriter(socket.getOutputStream(), true);

		String uzenet = "";
		String szerveruzenet = "";
		String tabla = "";
		
		System.out.println();
		while(true)
		{
			if(szerveruzenet.equals("VARAKOZAS"))
			{
				System.out.println("Várakozás a második játékos csatlakozására.");
				
			}
			else if(szerveruzenet.equals("TABLA"))
			{
				szerveruzenet = socketIn.readLine();
				System.out.println(szerveruzenet);
				//kirajzol(szerveruzenet);
			}
			else if(szerveruzenet.equals("HAJOID"))
			{
				do
				{
					System.out.println("Tedd le a hajót:");
				}
				while(!Bemenet((uzenet = stdIn.readLine())));
				socketOut.println(uzenet);
			}
			else if(szerveruzenet.equals("JATEK_VEGE"))
			{
				szerveruzenet = socketIn.readLine();
				System.out.println(szerveruzenet);

				szerveruzenet = socketIn.readLine();
				System.out.println(szerveruzenet);

				szerveruzenet = socketIn.readLine();
				System.out.println(szerveruzenet);
			break;
			}
		}

		
		
		
		System.out.println("Kapcsolat bontasa.");
		socketOut.close();
		stdIn.close();
		socketIn.close();
		socket.close();

	}
}
