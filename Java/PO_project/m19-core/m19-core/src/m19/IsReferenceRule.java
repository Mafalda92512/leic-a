package m19;


public class IsReferenceRule implements Rule {

    private Work _work;

    public IsReferenceRule(Work work){
        _work = work;
    }
    
    public int test() {
        if (_work.isReference())
            return 5;
        return 0;
    }
}