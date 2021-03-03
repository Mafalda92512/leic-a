package m19;

import java.io.Serializable;

public class Excelent extends Behaviour implements Serializable{

  /**
   *
   */
  private static final long serialVersionUID = -8688566763562389860L;
  private int _state;
  private User _user;

  public Excelent(User user) {
    super("CUMPRIDOR", 5);
    _user = user;
  }

  public int calcTime(int copies){
    if(copies == 1) return 8;
    if(copies <=5) return 15;
    if(copies > 5) return 30;
    return 0;
  }


  public void calcRegularity() {
    if (_state == 0) _user.setNormal(-1);
  }

  public void decrementRegularity() {
    _state = 0;
  }

  public void incrementRegularity() {
  }

}
