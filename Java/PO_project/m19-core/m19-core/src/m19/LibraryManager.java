package m19;

import java.io.IOException;

import m19.exceptions.MissingFileAssociationException;
import m19.exceptions.FailedToOpenFileException;
import m19.exceptions.ImportFileException;
import m19.exceptions.BadEntrySpecificationException;
import m19.exceptions.UserNotFoundException;
import m19.exceptions.WorkNotFoundException;

import java.io.ObjectOutputStream;
import java.io.FileOutputStream;
import java.io.BufferedOutputStream;

import java.io.ObjectInputStream;
import java.io.FileInputStream;
import java.io.BufferedInputStream;
import java.util.ArrayList;


/**
 * The façade class.
 */
public class LibraryManager {

  private Library _library;
  private String _filename;

  public LibraryManager() {
    _library = new Library();
  }

  /**
   * @throws MissingFileAssociationException
   * @throws IOException
   * @throws FileNotFoundException
   */
  public void save() throws IOException, MissingFileAssociationException {
    if (_library.getChange() == 1) {
      try {
        if (filenameEmpty())
          throw new MissingFileAssociationException();
        ObjectOutputStream oos = new ObjectOutputStream(new BufferedOutputStream(new FileOutputStream(_filename)));
        oos.writeObject(_library);
        oos.close();
        _library.setChange(0);
      } catch (IOException IOException) {
        IOException.printStackTrace();
      }
    }
  }

  public int displayDate() {
    return _library.displayDate();
  }

  public Request getRequest(User user, Work work){
    return _library.getRequest(user, work);
  }

  public void advanceDate(int daysToAdvance) {
    _library.advanceDate(daysToAdvance);
  }

  public void payFine(User user) {
    _library.payFine(user);
  }

  public void updateRequest(User user, Work work) {
    _library.updateRequest(user, work);
  }

  public int requestWork(User user, Work work) {
    int time = this.calcTime(user, work.getCopies());
    return _library.requestWork(user, work, time);
  }

  public void returnWork(User user, Work work) {
    _library.returnWork(user, work);
  }

  public User registerUser(String name, String email) {
    return _library.registerUser(name, email);
  }

  public Boolean isBorrowed(User user, Work work) {
    return _library.isBorrowed(user, work);
  }

  public ArrayList<Notification> showUserNotifications(int userID) throws UserNotFoundException{
    return _library.showUserNotifications(userID);
  }

  public void CleanNotificationsList(int userID) {
    _library.cleanNotifications(userID);
  }

  public void requestsNotification(int workID, User userToNotify) {
    _library.requestsNotification(workID, userToNotify);
  }

  public User showUser(int userID) throws UserNotFoundException{
    return _library.showUser(userID);
  }

  public ArrayList<User> showAllUsers() {
    return _library.showAllUsers();
  }

  public ArrayList<Work> displayAllWorks() {
    return _library.displayAllWorks();
  }

  public Work displayWork(int workID) throws WorkNotFoundException{
    return _library.displayWork(workID);
  }

  public ArrayList<Work> performSearch(String searchTerm) {
    return _library.performSearch(searchTerm);
  }

  public int calcTime(User user, int copies) {
    return _library.displayDate() + _library.returnTime(user, copies);
  }

  /**
   * @param filename
   * @throws MissingFileAssociationException
   * @throws IOException
   */
  public void saveAs(String filename) {
    _filename = filename;
    try {
      save();
    } catch (MissingFileAssociationException MissingFileAssociationException) {
      MissingFileAssociationException.printStackTrace();
    } catch (IOException IOException) {
      IOException.printStackTrace();
    }
  }

  public Boolean filenameEmpty() {
    return _filename == null;
  }

  /**
   * @param filename
   * @throws FailedToOpenFileException
   * @throws IOException
   * @throws ClassNotFoundException
   */
  public void load(String filename) throws FailedToOpenFileException, IOException, ClassNotFoundException {
    _filename = filename;
    ObjectInputStream in = new ObjectInputStream(new BufferedInputStream(new FileInputStream(filename)));
    _library = (Library) in.readObject();
    in.close();
    _library.setChange(0);
  }

  /**
   * @param datafile
   * @throws ImportFileException
   */
  public void importFile(String datafile) throws ImportFileException {
    try {
      _library.importFile(datafile);
    } catch (IOException | BadEntrySpecificationException e) {
      throw new ImportFileException(e);
    }
  }

}
