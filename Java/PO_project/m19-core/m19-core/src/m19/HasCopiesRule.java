package m19;


public class HasCopiesRule implements Rule {

    private Work _work;
    
    public HasCopiesRule(Work work){
        _work = work;
    }
    public int test() {
        if (_work.hasCopies())
            return 3;
        return 0;
    }
}