package m19;

import java.io.Serializable;
import java.util.ArrayList;

public abstract class Work implements Serializable, Subject {
  /**
   *
   */
  private static final long serialVersionUID = -5357999796908740977L;
  private int _idWork;
  private int _numCopies;
  private int _numDisp;
  private int _price;
  private String _title;
  private String _category;
  private ArrayList<Observer> _observers = new ArrayList<Observer>(); // lista de users interessados na obra
  private Boolean _status = true; // passa a false quando _numDisp = 0

  public Work (int idWork,  int numCopies, String title, int price, String category) {
    _idWork = idWork;
    _numCopies = numCopies;
    _price = price;
    _numDisp = numCopies;
    _title = title;
    _category = category;
  }

  public abstract Boolean searchTitle(String searchTerm);

  public abstract Boolean searchAuthorDirector(String searchTerm);

  public void decrementCopies(){
    _numDisp -= 1;
  }

  public void incrementCopies() {
    _numDisp += 1;
  }

  public Boolean hasCopies(){
    return _numDisp<=0;
  }

  public Boolean isReference(){
    return _category.equals("REFERENCE");
  }

  public Boolean priceNotAllowed(){
    return _price > 25;
  }

  public int getId() {
    return _idWork;
  }

  public int getDisp(){
    return _numDisp;
  }

  public int getCopies(){
    return _numCopies;
  }

  public String getTitle(){
    return _title;
  }

  public int getPrice(){
    return _price;
  }

  public String getCategory(){
    for (Category c : Category.values()){
      if(_category.equals(c.toString())) return c.getCategoryEnum();
    }
    return _category;
  }

  public Boolean checkStatusChange(){
    if(_numDisp == 0) return false;
    return true;
  }

  public String toString1(){
    return this.getId()+" - "+this.getDisp()+" de "+ this.getCopies();
  }

  public String toString2(){
    return this.getTitle() +" - "+ this.getPrice()+" - "+this.getCategory();
  }

  public void changeStatus(Boolean b) {
    _status = b;
    statusChanged();
  }

  public void statusChanged() {
    if (_status == true)
      notifyObservers();
  }

  public void checkStatus() {
    _status = checkStatusChange();
    statusChanged();
  }

  public void registerObserver(Observer o) {
    _observers.add(o);
  }

  public void removeObservers() {
    _observers = new ArrayList<Observer>();
  }

  public void notifyObservers() {
    for (Observer observer : _observers) {
      observer.update(toString());
    }
    removeObservers();
  }


}
