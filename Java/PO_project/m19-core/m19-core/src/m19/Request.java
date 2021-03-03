package m19;

import java.io.Serializable;

public class Request implements Serializable{
  
  /**
   *
   */
  private static final long serialVersionUID = -3295396263012164784L;
  private int _deliveryDate;
  private int _fine;
  private User _user;
  private Work _work;

  public Request(int diaEntrega, User user, Work work){
    _deliveryDate = diaEntrega;
    _user = user;
    _work = work;
  }

  public void calcFine(int date){
    if(_deliveryDate < date);
    int mult = date - _deliveryDate;
    _fine = 5*mult;
    _user.checkStatus();
  }

  public int getFine(){
    return _fine;
  }

  public User getUser(){
    return _user;
  }

  public Work getWork(){
    return _work;
  }

  public int getDeliveryDay(){
    return _deliveryDate;
  }

  public void returnRequest(int date){
    if (_deliveryDate < date) {
      _user.setSuspended();
      _user.badReturn();
      // notify user
    }
    _user.goodReturn();
  }
  
  public void updateRequest(int date){
    if(_deliveryDate < date){
      _user.setSuspended();
      calcFine(date);
    }
  }

  public void setFine(int fine){
    _fine = fine;
  }
}
