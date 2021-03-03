package m19;

public class HasAlreadyRequestedRule implements Rule{

    private User _user;
    private Work _work;

    public HasAlreadyRequestedRule(User user, Work work){
        _user = user;
        _work = work;
    }
    public int test() {
        if (_user.hasAlreadyRequested(_work))
            return 1;
        return 0;
    }
}