public enum Product {
    Food,
    Electronics,
    Luxury;
    public char abbreviate(){
        switch(this){
            case Food:
                return 'F';
            case Electronics:
                return 'E';
            default:
                return 'L';



        }

    }
}
