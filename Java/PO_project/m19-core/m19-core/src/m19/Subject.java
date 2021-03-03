package m19;

public interface Subject {
  public abstract void registerObserver(Observer o);


  public abstract void notifyObservers();
}