package m19;

import java.io.Serializable;

public class Normal extends Behaviour implements Serializable{

  /**
   *
   */
  private static final long serialVersionUID = -4588164967715301854L;
  private int _state;
  private User _user;

  public Normal(int state, User user) {
    super("NORMAL", 3);
    _state = state;
    _user = user;
  }

  public int calcTime(int copies){
    if(copies == 1) return 3;
    if(copies <=5) return 8;
    if(copies > 5) return 15;
    return 0;
  }

  public void calcRegularity(){
    if (_state == 5) _user.setExcelent();
    if(_state == -3) _user.setFaulty();
  }

  public void decrementRegularity(){
    if (_state >= 0)  _state = 0;
    _state -= 1;
  }

  public void incrementRegularity(){
    if(_state <=0) _state = 0;
    _state += 1;
  }
}
