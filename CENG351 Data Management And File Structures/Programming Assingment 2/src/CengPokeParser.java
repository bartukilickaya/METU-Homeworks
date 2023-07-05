import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Scanner;

public class CengPokeParser {

	public static ArrayList<CengPoke> parsePokeFile(String filename)
	{
		ArrayList<CengPoke> pokeList = new ArrayList<CengPoke>();
		String line;
		String[] temp;
		Integer pokeKey;
		String pokeName;
		String pokePower;
		String pokeType;
		try{
			FileReader input = new FileReader(filename);
			BufferedReader bufferreader = new BufferedReader(input);
			while ((line = bufferreader.readLine()) != null) {
				//System.out.println(line);
				temp = line.split("\t");
				pokeKey = Integer.parseInt(temp[1]);
				pokeName = temp[2];
				pokePower = temp[3];
				pokeType = temp[4];
				CengPoke cengpoke = new CengPoke(pokeKey,pokeName,pokePower,pokeType);
				pokeList.add(cengpoke);

			}
		}
		catch(Exception e) {
			e.getStackTrace();
		}

		return pokeList;
	}
	
	public static void startParsingCommandLine() throws IOException
	{
		Scanner input = new  Scanner(System.in);
		String tmp = "";
		int pokeKey;
		String pokeName,pokePower,pokeType;
		CengPoke cengpoke;
		while (input.hasNext()) {
			tmp = input.next();
			if( tmp.equalsIgnoreCase("add") ) {
				tmp = input.next();
				pokeKey = Integer.parseInt(tmp);
				tmp = input.next();
				pokeName = tmp;
				tmp = input.next();
				pokePower = tmp;
				tmp = input.next();
				pokeType = tmp;
				cengpoke = new CengPoke(pokeKey, pokeName, pokePower, pokeType);
				CengPokeKeeper.addPoke(cengpoke);
			}
			if (tmp.equalsIgnoreCase("quit")) {
				System.exit(0);
			}
			if (tmp.equalsIgnoreCase("search")) {
				tmp = input.next();
				pokeKey = Integer.parseInt(tmp);
				CengPokeKeeper.searchPoke(pokeKey);
			}
			if (tmp.equalsIgnoreCase("delete")) {
				tmp = input.next();
				pokeKey = Integer.parseInt(tmp);
				CengPokeKeeper.deletePoke(pokeKey);
			}
			if (tmp.equalsIgnoreCase("print")) {
				CengPokeKeeper.printEverything();
			}
		}
	}
}
