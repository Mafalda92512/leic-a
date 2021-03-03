package m19;

import java.util.List;
import java.util.ArrayList;
import java.util.Comparator;

import java.io.Serializable;


public class User implements Serializable, Observer {

  private static final long serialVersionUID = 3321115925123693242L;
  private int _idUser;
  private int _fine;
  private String _situation;
  private String _name;
  private String _email;
  private Behaviour _behaviour;
  private List<Work> _borrowed = new ArrayList<Work>();
  private ArrayList<Notification> _notifications = new ArrayList<Notification>();

  public static Comparator<User> userComparator = new Comparator<User>() {
    public int compare(User u1, User u2) {
      String UserName1 = u1.getName().toLowerCase();
      String UserName2 = u2.getName().toLowerCase();

      if (UserName1.equals(UserName2)) {
        return u1.getId() - u2.getId();
      }
      return UserName1.compareTo(UserName2);

    }
  };

/*_______________________________CONSTRUTOR___________________________________*/

  public User (int numUser, String name, String email) {
    _idUser = numUser;
    _name = name;
    _email = email;
    _fine = 0;
    _situation = "ACTIVO";
    _behaviour = new Normal(0, this);
  }

/*____________________________________________________________________________*/
/*_____________________________GETTERS E SETTERS______________________________*/

  public String getName(){
    return _name;
  }

  public String getEmail(){
    return _email;
  }

  public int getId() {
    return _idUser;
  }

  public ArrayList<Notification> getNotifications() {
    return _notifications;
  }

  public int getFineValue(){
    return _fine;
  }

  public String getSituation(){
    return _situation;
  }

  public void setSuspended(){
    _situation = "SUSPENSO";
  }

  public void setActive(){
    _situation = "ACTIVO";
  }

  public String getBehaviour(){
    return _behaviour.getBehaviour();
  }

  public void setNormal(int normal){
    _behaviour = new Normal(normal, this);
  }

  public void setFaulty() {
    _behaviour = new Faulty(this);
  }

  public void setExcelent() {
    _behaviour = new Excelent(this);
  }

/*____________________________________________________________________________*/
/*_________________________________REQUESTS___________________________________*/


  public Boolean addRequest(Work work, Request r){
    _borrowed.add(work);
    return true;
  }

  public void removeRequest(Work work){
    _borrowed.remove(work);
    this.checkStatus();
  }

  public void badReturn(){
    _behaviour.decrementRegularity();
    _behaviour.calcRegularity();
  }

  public void goodReturn(){
    _behaviour.incrementRegularity();
   _behaviour.calcRegularity();
  }

/*____________________________________________________________________________*/
/*__________________________________REGRAS____________________________________*/

  public Boolean hasAlreadyRequested(Work work){
    for (int i = 0; i < _borrowed.size(); i++) {
      if(_borrowed.contains(work)) return true;
    }
    return false;
  }

  public Boolean hasWorkRequested(Work work){
    for(int i = 0; i < _borrowed.size(); i++){
      if(_borrowed.contains(work)) return true;
    }
    return false;
  }

  public void checkStatus(){
    if(_fine != 0) return;
    if(_borrowed.size() != 0) return;
    this.setActive();
  }
  
  public Boolean canRequest(){
    return this.getSituation().equals("ACTIVO");
  }

  public int returnTime(int copies){
    return _behaviour.calcTime(copies);
  }

  public int calcReturnTime(int copies){
    return _behaviour.calcTime(copies);
  }

  public Boolean isSuspended(){
    return _situation.equals("SUSPENSO");
  }

  public Boolean maxRequested(){
    return _borrowed.size() < _behaviour.maxRequests();
  }

/*____________________________________________________________________________*/
/*________________________________NOTIFICACOES________________________________*/

  public void addNotification(String notification) {
    _notifications.add(new NotificationReturn(notification));
  }

  public void cleanUserNotifications() {
    _notifications = new ArrayList<Notification>();
  }

  public void update(String workDescription) {
    addNotification(workDescription);
  }

/*____________________________________________________________________________*/
/*___________________________________MULTA____________________________________*/

  public void payFine() {
    _fine = 0;
    checkStatus();
  }


  public void notPayFine(int total){
    _fine += total;
    this.setSuspended();
  }

/*____________________________________________________________________________*/
/*________________________________TO STRING___________________________________*/

  @Override
  public String toString(){
    checkStatus();
    if(!this.isSuspended()){
      return this.getId()+" - "+this.getName()+" - "+this.getEmail()+" - "+this.getBehaviour()+" - "+ this.getSituation();
    }
    else{
      return this.getId()+" - "+this.getName()+" - "+this.getEmail()+" - "+this.getBehaviour()+" - "+ this.getSituation()+" - EUR "+ this.getFineValue();
    }
  }
}
/*____________________________________________________________________________*/
