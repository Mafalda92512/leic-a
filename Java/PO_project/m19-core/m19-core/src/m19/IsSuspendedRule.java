package m19;


public class IsSuspendedRule implements Rule {

    private User _user;
    
    public IsSuspendedRule(User user){
        _user = user;
    }
    
    public int test() {
        if (_user.isSuspended()) return 2;
        return 0;
    }
}