package m19;


public class MaxRequestedRule implements Rule {

    private User _user;

    public MaxRequestedRule(User user){
        _user = user;
    }
    
    public int test() {
        if (!(_user.maxRequested()))
            return 4;
        return 0;
    }
}