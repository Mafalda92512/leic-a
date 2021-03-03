package m19;

import java.util.ArrayList;

public class RuleComposite implements Rule {

    private ArrayList<Rule> _rules = new ArrayList<Rule>();
    private int resultado;

    public RuleComposite(User user, Work work){
        _rules.add(new HasAlreadyRequestedRule(user, work));
        _rules.add(new IsSuspendedRule(user));
        _rules.add(new HasCopiesRule(work));
        _rules.add(new MaxRequestedRule(user));
        _rules.add(new IsReferenceRule(work));
        _rules.add(new PriceNotAllowedRule(work));
        user.checkStatus();
    }
    
    public int test(){
        for(int i = 0; i<_rules.size() ; i++){
           resultado = _rules.get(i).test();
           if(resultado != 0) break;
        }
        return resultado;
    }
}