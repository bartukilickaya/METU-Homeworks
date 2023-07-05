import java.lang.Math;
import java.util.ArrayList;

public class CengHashTable {
	Integer globaldepth = 0;
	Row [] row_array = new Row[1];
	public class Row{
		Bucket pointer;
		public Row(Bucket bucket){
			pointer = bucket;
		}
	}
	public class Bucket{
		Integer localdepth = 0;
		Integer bucketSize = CengPokeKeeper.getBucketSize();
		Integer actualsize = 0;
		CengPoke [] bucket = new CengPoke[bucketSize];
		public Bucket(){
			//pass
		}
		public Bucket(Integer localdepth){
			this.localdepth = localdepth;
		}
	}
	public CengHashTable()
	{
		Bucket bucket = new Bucket();
		row_array[0] = new Row(bucket);
	}

	public void deletePoke(Integer pokeKey)
	{
		// TODO: Empty Implementation
		int length = row_array.length,num_of_empty_buckets = 0,eachsize = 0,k = 0;
		ArrayList<Bucket> arr = new ArrayList<>();
		for(int i = 0 ; i < length; i++){
			if( row_array[i].pointer != null ){
				eachsize = row_array[i].pointer.actualsize;
				for(int j = 0; j < eachsize; j++){
					if( row_array[i].pointer.bucket[j].pokeKey().compareTo(pokeKey) == 0){
						for(k = j; k < eachsize-1; k++){
							row_array[i].pointer.bucket[k] = row_array[i].pointer.bucket[k+1];
						}
						row_array[i].pointer.bucket[k] = null;
						row_array[i].pointer.actualsize -= 1;
						break;
					}
				}
			}
		}
		//int flag;
		for(int i = 0 ; i < length; i++){
			if( !arr.contains(row_array[i].pointer) ){
				arr.add(row_array[i].pointer);
			}
		}
		for(int i = 0 ; i < arr.size(); i++){
			if( arr.get(i).actualsize == 0){
				num_of_empty_buckets += 1;
			}
		}
		System.out.println("\"delete\": {");
		System.out.println("\t\"emptyBucketNum\": " + num_of_empty_buckets);
		System.out.println("}");
	}

	public void addPoke(CengPoke poke)
	{
		Integer hashmod = CengPokeKeeper.getHashMod();
		Integer binary_value = poke.pokeKey() % hashmod;
		binary_value = binary_value >> ((int)((Math.log(hashmod) / Math.log(2)) - globaldepth));
		Integer row_index = Integer.parseInt(String.valueOf(binary_value));
		Integer new_row_index;
		int i = 0,j = 0,count = 0;
		while(true){
			if(row_array[row_index].pointer.actualsize ==  row_array[row_index].pointer.bucketSize ){ // bucket_full
				if( row_array[row_index].pointer.localdepth == globaldepth ){ // expand directory
					Row []tmp_array = row_array; // temp directory
					row_array = new Row[(int)Math.pow(2,globaldepth+1)];
					i = 0;
					j = 0;
					while(i < Math.pow(2,globaldepth)){ // set new directory
						row_array[j] = new Row(tmp_array[i].pointer);
						row_array[j+1] = new Row(tmp_array[i].pointer);
						i+=1;
						j+=2;
					}
					globaldepth+=1;
					row_index = row_index << 1;
					row_array[row_index].pointer.localdepth += 1;
					Bucket temp1 = new Bucket(row_array[row_index].pointer.localdepth);
					Bucket temp2 = new Bucket(row_array[row_index].pointer.localdepth);
					Bucket tmp_ptr = row_array[row_index].pointer;
					Integer shift_factor = globaldepth - (row_array[row_index].pointer.localdepth-1);
					new_row_index = (row_index >> shift_factor) << shift_factor ; // go to first pointer of that bucket (lowest number)
					count = 0;
					while( count < (int) (Math.pow(2,globaldepth - (tmp_ptr.localdepth-1)) /2) ){ // 2^gl-lo/2 many pointers-->temp1
						row_array[new_row_index].pointer = temp1;
						new_row_index += 1;
						count+= 1;
					}
					while( count < (int)(Math.pow(2,globaldepth - (tmp_ptr.localdepth-1))) ){ // rest of pointers -->temp2
						row_array[new_row_index].pointer = temp2;
						new_row_index += 1;
						count+= 1;
					}
					i = 0;
					//System.out.println(tmp_ptr.bucket[0].fullName());
					while( i < tmp_ptr.bucketSize ){ // loop through bucket, relist them.
						Integer poke_key = tmp_ptr.bucket[i].pokeKey();
						binary_value = poke_key % hashmod;
						binary_value = binary_value >> ((int)((Math.log(hashmod) / Math.log(2)) - globaldepth));
						new_row_index = Integer.parseInt(String.valueOf(binary_value));
						row_array[new_row_index].pointer.bucket[row_array[new_row_index].pointer.actualsize] = tmp_ptr.bucket[i]; // put poke
						row_array[new_row_index].pointer.actualsize += 1; // increase actualsize
						i+=1;
					}
					hashmod = CengPokeKeeper.getHashMod();
					binary_value = poke.pokeKey() % hashmod;
					binary_value = binary_value >> ((int)((Math.log(hashmod) / Math.log(2)) - globaldepth));
					row_index = Integer.parseInt(String.valueOf(binary_value));
					if( row_array[row_index].pointer.actualsize == tmp_ptr.bucketSize ){ // if we still have full bucket even though we split, continue the loop
						continue;
					}
					else{
						row_array[row_index].pointer.bucket[row_array[row_index].pointer.actualsize] = poke;
						row_array[row_index].pointer.actualsize += 1;
						break;
					}
				}
				else{ // local < global
					//System.out.println("3");
					row_array[row_index].pointer.localdepth += 1;
					Bucket temp1 = new Bucket(row_array[row_index].pointer.localdepth);
					Bucket temp2 = new Bucket(row_array[row_index].pointer.localdepth);
					Bucket tmp_ptr = row_array[row_index].pointer;
					Integer shift_factor = globaldepth - (row_array[row_index].pointer.localdepth-1);
					new_row_index = (row_index >> shift_factor) << shift_factor ; // go to first pointer of that bucket (lowest number)
					count = 0;
					while( count < (int) (Math.pow(2,globaldepth - (tmp_ptr.localdepth-1)) /2) ){ // 2^gl-lo/2 many pointers-->temp1
						row_array[new_row_index].pointer = temp1;
						new_row_index += 1;
						count+= 1;
					}
					while( count < (int)(Math.pow(2,globaldepth - (tmp_ptr.localdepth-1))) ){ // rest of pointers -->temp2
						row_array[new_row_index].pointer = temp2;
						new_row_index += 1;
						count+= 1;
					}
					i = 0;
					while( i < tmp_ptr.bucketSize ){ // loop through bucket, relist them.
						Integer poke_key = tmp_ptr.bucket[i].pokeKey();
						binary_value = poke_key % hashmod;
						binary_value = binary_value >> ((int)((Math.log(hashmod) / Math.log(2)) - globaldepth));
						new_row_index = Integer.parseInt(String.valueOf(binary_value));
						row_array[new_row_index].pointer.bucket[row_array[new_row_index].pointer.actualsize] = tmp_ptr.bucket[i]; // put poke
						row_array[new_row_index].pointer.actualsize += 1; // increase actualsize
						i+=1;
					}
					hashmod = CengPokeKeeper.getHashMod();
					binary_value = poke.pokeKey() % hashmod;
					binary_value = binary_value >> ((int)((Math.log(hashmod) / Math.log(2)) - globaldepth));
					row_index = Integer.parseInt(String.valueOf(binary_value));
					if( row_array[row_index].pointer.actualsize == tmp_ptr.bucketSize ){ // if we still have full bucket even though we split, continue the loop
						continue;
					}
					else{
						row_array[row_index].pointer.bucket[row_array[row_index].pointer.actualsize] = poke;
						row_array[row_index].pointer.actualsize += 1;
						break;
					}
				}
			}
			else{ // bucket is not full
				//System.out.println("4");
				row_array[row_index].pointer.bucket[row_array[row_index].pointer.actualsize] = poke;
				row_array[row_index].pointer.actualsize+= 1;
				break;
			}
		}
		//System.out.print("gameover");

	}
	
	public void searchPoke(Integer pokeKey)
	{
		// TODO: Empty Implementation
		System.out.println("\"search\": {");
		int lengthh = row_array.length,each_bucket_size,count = 0,length = 0;
		int i = 0,j = 0,c = 0, k = 0;
		char [] hash_prefix = new char[globaldepth];
		while(k < globaldepth){
			hash_prefix[k] = '0';
			k+=1;
		}
		for(i = 0; i < lengthh; i++){
			if(row_array[i].pointer != null){
				each_bucket_size = row_array[i].pointer.actualsize;
				for(j = 0; j < each_bucket_size;j++){
					if( row_array[i].pointer.bucket[j].pokeKey().compareTo(pokeKey) == 0){
						count += 1;
					}
				}
			}
		}
		for(i = 0; i < lengthh && count > 1; i++){
			if(row_array[i].pointer != null){
				each_bucket_size = row_array[i].pointer.actualsize;
				for(j = 0; j < each_bucket_size;j++){
					if( row_array[i].pointer.bucket[j].pokeKey().compareTo(pokeKey) == 0 ){
						System.out.println("\t\"row\": {");
						String tmp = Integer.toBinaryString(i);
						length = tmp.length();
						k = 0;
						if( length > globaldepth ){
							while(k < globaldepth){
								hash_prefix[hash_prefix.length-k-1] = tmp.charAt(length-1-k);
								k+=1;
							}
						}
						else{
							while(k < length){
								hash_prefix[hash_prefix.length-k-1] = tmp.charAt(length-1-k);
								k+=1;
							}
						}
						System.out.print("\t\t\"hashPref\": " + String.valueOf(hash_prefix) +",\n");
						System.out.print("\t\t\"bucket\": {\n");
						System.out.print("\t\t\t\"hashLength\": " + String.valueOf(row_array[i].pointer.localdepth) + ",\n");
						System.out.print("\t\t\t\"pokes\": [\n");
						int t = 0;
						for(t = 0; t < each_bucket_size-1; t++){
							System.out.print("\t\t\t\t\"poke\": {\n");
							Integer h = row_array[i].pointer.bucket[t].pokeKey() % CengPokeKeeper.getHashMod();
							char []hash = new char[(int)(Math.log(CengPokeKeeper.getHashMod()) / Math.log(2))];
							k = 0;
							while(k < hash.length){
								hash[k] = '0';
								k+= 1;
							}
							tmp = Integer.toBinaryString(h);
							length = tmp.length();
							k = 0;
							while( k < length ){
								hash[hash.length-1-k] = tmp.charAt(length-k-1);
								k+= 1;
							}
							System.out.print("\t\t\t\t\t\"hash\": " + String.valueOf(hash) +",\n");
							System.out.print("\t\t\t\t\t\"pokeKey\": " + String.valueOf(row_array[i].pointer.bucket[t].pokeKey()) + ",\n");
							System.out.print("\t\t\t\t\t\"pokeName\": " + row_array[i].pointer.bucket[t].pokeName() + ",\n");
							System.out.print("\t\t\t\t\t\"pokePower\": " + row_array[i].pointer.bucket[t].pokePower() + ",\n");
							System.out.print("\t\t\t\t\t\"pokeType\": " + row_array[i].pointer.bucket[t].pokeType() + "\n");
							System.out.print("\t\t\t\t},\n");
						}
						System.out.print("\t\t\t\t\"poke\": {\n");
						Integer h = row_array[i].pointer.bucket[t].pokeKey() % CengPokeKeeper.getHashMod();
						char []hash = new char[(int)(Math.log(CengPokeKeeper.getHashMod()) / Math.log(2))];
						k = 0;
						while(k < hash.length){
							hash[k] = '0';
							k+= 1;
						}
						tmp = Integer.toBinaryString(h);
						length = tmp.length();
						k = 0;
						while( k < length ){
							hash[hash.length-1-k] = tmp.charAt(length-k-1);
							k+= 1;
						}
						System.out.print("\t\t\t\t\t\"hash\": " + String.valueOf(hash) +",\n");
						System.out.print("\t\t\t\t\t\"pokeKey\": " + String.valueOf(row_array[i].pointer.bucket[t].pokeKey()) + ",\n");
						System.out.print("\t\t\t\t\t\"pokeName\": " + row_array[i].pointer.bucket[t].pokeName() + ",\n");
						System.out.print("\t\t\t\t\t\"pokePower\": " + row_array[i].pointer.bucket[t].pokePower() + ",\n");
						System.out.print("\t\t\t\t\t\"pokeType\": " + row_array[i].pointer.bucket[t].pokeType() + "\n");
						System.out.print("\t\t\t\t}\n");
						System.out.println("\t\t\t]");
						System.out.println("\t\t}");
						System.out.println("\t},");
						count -=1;
						break;
					}
				}
				if( count == 1){
					i+=1;
					break;
				}
			}
		}
		for(; i < lengthh; i++){
			if(row_array[i].pointer != null){
				each_bucket_size = row_array[i].pointer.actualsize;
				for(j = 0; j < each_bucket_size;j++){
					if( row_array[i].pointer.bucket[j].pokeKey().compareTo(pokeKey) == 0 ){
						System.out.println("\t\"row\": {");
						String tmp = Integer.toBinaryString(i);
						length = tmp.length();
						k = 0;
						if( length > globaldepth ){
							while(k < globaldepth){
								hash_prefix[hash_prefix.length-k-1] = tmp.charAt(length-1-k);
								k+=1;
							}
						}
						else{
							while(k < length){
								hash_prefix[hash_prefix.length-k-1] = tmp.charAt(length-1-k);
								k+=1;
							}
						}
						System.out.print("\t\t\"hashPref\": " + String.valueOf(hash_prefix) +",\n");
						System.out.print("\t\t\"bucket\": {\n");
						System.out.print("\t\t\t\"hashLength\": " + String.valueOf(row_array[i].pointer.localdepth) + ",\n");
						System.out.print("\t\t\t\"pokes\": [\n");
						int t = 0;
						for(t = 0; t < each_bucket_size-1; t++){
							System.out.print("\t\t\t\t\"poke\": {\n");
							Integer h = row_array[i].pointer.bucket[t].pokeKey() % CengPokeKeeper.getHashMod();
							char []hash = new char[(int)(Math.log(CengPokeKeeper.getHashMod()) / Math.log(2))];
							k = 0;
							while(k < hash.length){
								hash[k] = '0';
								k+= 1;
							}
							tmp = Integer.toBinaryString(h);
							length = tmp.length();
							k = 0;
							while( k < length ){
								hash[hash.length-1-k] = tmp.charAt(length-k-1);
								k+= 1;
							}
							System.out.print("\t\t\t\t\t\"hash\": " + String.valueOf(hash) +",\n");
							System.out.print("\t\t\t\t\t\"pokeKey\": " + String.valueOf(row_array[i].pointer.bucket[t].pokeKey()) + ",\n");
							System.out.print("\t\t\t\t\t\"pokeName\": " + row_array[i].pointer.bucket[t].pokeName() + ",\n");
							System.out.print("\t\t\t\t\t\"pokePower\": " + row_array[i].pointer.bucket[t].pokePower() + ",\n");
							System.out.print("\t\t\t\t\t\"pokeType\": " + row_array[i].pointer.bucket[t].pokeType() + "\n");
							System.out.print("\t\t\t\t},\n");
						}
						System.out.print("\t\t\t\t\"poke\": {\n");
						Integer h = row_array[i].pointer.bucket[t].pokeKey() % CengPokeKeeper.getHashMod();
						char []hash = new char[(int)(Math.log(CengPokeKeeper.getHashMod()) / Math.log(2))];
						k = 0;
						while(k < hash.length){
							hash[k] = '0';
							k+= 1;
						}
						tmp = Integer.toBinaryString(h);
						length = tmp.length();
						k = 0;
						while( k < length ){
							hash[hash.length-1-k] = tmp.charAt(length-k-1);
							k+= 1;
						}
						System.out.print("\t\t\t\t\t\"hash\": " + String.valueOf(hash) +",\n");
						System.out.print("\t\t\t\t\t\"pokeKey\": " + String.valueOf(row_array[i].pointer.bucket[t].pokeKey()) + ",\n");
						System.out.print("\t\t\t\t\t\"pokeName\": " + row_array[i].pointer.bucket[t].pokeName() + ",\n");
						System.out.print("\t\t\t\t\t\"pokePower\": " + row_array[i].pointer.bucket[t].pokePower() + ",\n");
						System.out.print("\t\t\t\t\t\"pokeType\": " + row_array[i].pointer.bucket[t].pokeType() + "\n");
						System.out.print("\t\t\t\t}\n");
						System.out.println("\t\t\t]");
						System.out.println("\t\t}");
						System.out.println("\t}");
						count -=1;
						break;
					}
				}
			}
		}
		System.out.println("}");
	}
	
	public void print()
	{
		System.out.print("\"table\": {\n");
		Integer i = 0,j = 0,k = 0,length = 0;
		char [] hash_prefix = new char[globaldepth];
		while(k < globaldepth){
			hash_prefix[k] = '0';
			k+=1;
		}
		while(i < Math.pow(2,this.globaldepth) -1) {
			System.out.print("\t\"row\": {\n");
			String tmp = Integer.toBinaryString(i);
			length = tmp.length();
			k = 0;
			if( length > globaldepth ){
				while(k < globaldepth){
					hash_prefix[hash_prefix.length-k-1] = tmp.charAt(length-1-k);
					k+=1;
				}
			}
			else{
				while(k < length){
					hash_prefix[hash_prefix.length-k-1] = tmp.charAt(length-1-k);
					k+=1;
				}
			}
			System.out.print("\t\t\"hashPref\": " + String.valueOf(hash_prefix) +",\n");
			System.out.print("\t\t\"bucket\": {\n");
			System.out.print("\t\t\t\"hashLength\": " + String.valueOf(row_array[i].pointer.localdepth) + ",\n");
			System.out.print("\t\t\t\"pokes\": [\n");
			j = 0;
			while(j < row_array[i].pointer.actualsize -1){
				System.out.print("\t\t\t\t\"poke\": {\n");
				Integer h = row_array[i].pointer.bucket[j].pokeKey() % CengPokeKeeper.getHashMod();
				char []hash = new char[(int)(Math.log(CengPokeKeeper.getHashMod()) / Math.log(2))];
				k = 0;
				while(k < hash.length){
					hash[k] = '0';
					k+= 1;
				}
				tmp = Integer.toBinaryString(h);
				length = tmp.length();
				k = 0;
				while( k < length ){
					hash[hash.length-1-k] = tmp.charAt(length-k-1);
					k+= 1;
				}
				System.out.print("\t\t\t\t\t\"hash\": " + String.valueOf(hash) +",\n");
				System.out.print("\t\t\t\t\t\"pokeKey\": " + String.valueOf(row_array[i].pointer.bucket[j].pokeKey()) + ",\n");
				System.out.print("\t\t\t\t\t\"pokeName\": " + row_array[i].pointer.bucket[j].pokeName() + ",\n");
				System.out.print("\t\t\t\t\t\"pokePower\": " + row_array[i].pointer.bucket[j].pokePower() + ",\n");
				System.out.print("\t\t\t\t\t\"pokeType\": " + row_array[i].pointer.bucket[j].pokeType() + "\n");
				System.out.print("\t\t\t\t},\n");
				j+=1;
			}
			if( row_array[i].pointer.actualsize != 0 ){
				Integer h = row_array[i].pointer.bucket[j].pokeKey() % CengPokeKeeper.getHashMod();
				char []hash = new char[(int)(Math.log(CengPokeKeeper.getHashMod()) / Math.log(2))];
				k = 0;
				while(k < hash.length){
					hash[k] = '0';
					k+= 1;
				}
				tmp = Integer.toBinaryString(h);
				length = tmp.length();
				k = 0;
				while( k < length ){
					hash[hash.length-1-k] = tmp.charAt(length-k-1);
					k+= 1;
				}
				System.out.print("\t\t\t\t\"poke\": {\n");
				System.out.print("\t\t\t\t\t\"hash\": " + String.valueOf(hash) +",\n");
				System.out.print("\t\t\t\t\t\"pokeKey\": " + String.valueOf(row_array[i].pointer.bucket[j].pokeKey()) + ",\n");
				System.out.print("\t\t\t\t\t\"pokeName\": " + row_array[i].pointer.bucket[j].pokeName() + ",\n");
				System.out.print("\t\t\t\t\t\"pokePower\": " + row_array[i].pointer.bucket[j].pokePower() + ",\n");
				System.out.print("\t\t\t\t\t\"pokeType\": " + row_array[i].pointer.bucket[j].pokeType() + "\n");
				System.out.print("\t\t\t\t}\n");
			}
			System.out.print("\t\t\t]\n");
			System.out.print("\t\t}\n");
			System.out.print("\t},\n");
			i+=1;
		}
		String tmp = Integer.toBinaryString(i);
		length = tmp.length();
		k = 0;
		if( length > globaldepth ){
			while(k < globaldepth){
				hash_prefix[hash_prefix.length-k-1] = tmp.charAt(length-1-k);
				k+=1;
			}
		}
		else{
			while(k < length){
				hash_prefix[hash_prefix.length-k-1] = tmp.charAt(length-1-k);
				k+=1;
			}
		}
		System.out.print("\t\"row\": {\n");
		System.out.print("\t\t\"hashPref\": " + String.valueOf(hash_prefix) + ",\n");
		System.out.print("\t\t\"bucket\": {\n");
		System.out.print("\t\t\t\"hashLength\": " + String.valueOf(row_array[i].pointer.localdepth) + ",\n");
		System.out.print("\t\t\t\"pokes\": [\n");
		j = 0;
		while(j < row_array[i].pointer.actualsize -1){
			System.out.print("\t\t\t\t\"poke\": {\n");
			Integer h = row_array[i].pointer.bucket[j].pokeKey() % CengPokeKeeper.getHashMod();
			char []hash = new char[(int)(Math.log(CengPokeKeeper.getHashMod()) / Math.log(2))];
			k = 0;
			while(k < hash.length){
				hash[k] = '0';
				k+= 1;
			}
			tmp = Integer.toBinaryString(h);
			length = tmp.length();
			k = 0;
			while( k < length ){
				hash[hash.length-1-k] = tmp.charAt(length-k-1);
				k+= 1;
			}
			System.out.print("\t\t\t\t\t\"hash\": "+ String.valueOf(hash)+",\n");
			System.out.print("\t\t\t\t\t\"pokeKey\": " + String.valueOf(row_array[i].pointer.bucket[j].pokeKey()) + ",\n");
			System.out.print("\t\t\t\t\t\"pokeName\": " + row_array[i].pointer.bucket[j].pokeName() + ",\n");
			System.out.print("\t\t\t\t\t\"pokePower\": " + row_array[i].pointer.bucket[j].pokePower() + ",\n");
			System.out.print("\t\t\t\t\t\"pokeType\": " + row_array[i].pointer.bucket[j].pokeType() + "\n");
			System.out.print("\t\t\t\t},\n");
			j+=1;
		}
		if( row_array[i].pointer.actualsize != 0){
			System.out.print("\t\t\t\t\"poke\": {\n");
			Integer h = row_array[i].pointer.bucket[j].pokeKey() % CengPokeKeeper.getHashMod();
			char []hash = new char[(int)(Math.log(CengPokeKeeper.getHashMod()) / Math.log(2))];
			k = 0;
			while(k < hash.length){
				hash[k] = '0';
				k+= 1;
			}
			tmp = Integer.toBinaryString(h);
			length = tmp.length();
			k = 0;

			while( k < length ){
				hash[hash.length-1-k] = tmp.charAt(length-k-1);
				k+= 1;
			}
			System.out.print("\t\t\t\t\t\"hash\": " + String.valueOf(hash)+ ",\n");
			System.out.print("\t\t\t\t\t\"pokeKey\": " + String.valueOf(row_array[i].pointer.bucket[j].pokeKey()) + ",\n");
			System.out.print("\t\t\t\t\t\"pokeName\": " + row_array[i].pointer.bucket[j].pokeName() + ",\n");
			System.out.print("\t\t\t\t\t\"pokePower\": " + row_array[i].pointer.bucket[j].pokePower() + ",\n");
			System.out.print("\t\t\t\t\t\"pokeType\": " + row_array[i].pointer.bucket[j].pokeType() + "\n");
			System.out.print("\t\t\t\t}\n");
			System.out.print("\t\t\t]\n");
			System.out.print("\t\t}\n");
			System.out.print("\t}\n");
		}
		else{
			System.out.print("\t\t\t]\n");
			System.out.print("\t\t}\n");
			System.out.print("\t}\n");
		}
		System.out.println("}");

	}

	// GUI-Based Methods
	// These methods are required by GUI to work properly.
	
	public int prefixBitCount()
	{
		// TODO: Return table's hash prefix length.
		return 0;		
	}
	
	public int rowCount()
	{
		// TODO: Return the count of HashRows in table.
		return 0;		
	}
	
	public CengHashRow rowAtIndex(int index)
	{
		// TODO: Return corresponding hashRow at index.
		return null;
	}
	
	// Own Methods
}
