package m19;

import java.io.Serializable;

public class Faulty extends Behaviour implements Serializable{

  /**
   *
   */
  private static final long serialVersionUID = 5905377767166302666L;
  private int _state;
  private User _user;

  public Faulty(User user) {
    super("FALTOSO",1);
    _user = user;
  }

  public int calcTime(int copies){
    return 2;
  }

  public void calcRegularity() {
    if (_state == 3) _user.setNormal(3);
  }

  public void decrementRegularity() {
    _state = 0;
  }

  public void incrementRegularity() {
    _state += 1;
  }

}
