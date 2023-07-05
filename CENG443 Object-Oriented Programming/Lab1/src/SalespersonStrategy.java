public enum SalespersonStrategy {
    Random,
    MiddleGround;
    public char abbreviate(){
        switch(this){
            case Random:
                return 'R';
            default:
                return 'M';

        }

    }
}
