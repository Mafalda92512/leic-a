package m19;

import java.io.Serializable;

public abstract class Behaviour implements Serializable{
  /**
   *
   */
  private static final long serialVersionUID = -8339844575261565530L;
  private String _behaviour;
  private int _maxRequests;

  public Behaviour(String behaviour, int maxRequests){
    _behaviour = behaviour;
    _maxRequests = maxRequests;
  }

  public String getBehaviour(){
    return _behaviour;
  }
  public int maxRequests(){
    return _maxRequests;
  }

  public abstract int calcTime(int copies);

  public abstract void calcRegularity();

  public abstract void decrementRegularity();

  public abstract  void incrementRegularity();
}
