package m19;

public class PriceNotAllowedRule implements Rule {

    private Work _work;

    public PriceNotAllowedRule(Work work){
        _work = work;
    }
    public int test(){
        if (_work.priceNotAllowed())
            return 6;
        return 0;
    }
}