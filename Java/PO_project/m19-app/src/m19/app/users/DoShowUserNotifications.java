package m19.app.users;

import m19.LibraryManager;
import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;
import m19.User;
import m19.app.exceptions.NoSuchUserException;
import m19.Notification;

import m19.exceptions.UserNotFoundException;


/**
 * 4.2.3. Show notifications of a specific user.
 */
public class DoShowUserNotifications extends Command<LibraryManager> {

  private Input<Integer> _userID;

  /**
   * @param receiver
   */
  public DoShowUserNotifications(LibraryManager receiver) {
    super(Label.SHOW_USER_NOTIFICATIONS, receiver);
    _userID = _form.addIntegerInput(Message.requestUserId());
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    _form.parse();
    try {
      for (Notification n : _receiver.showUserNotifications(_userID.value())) {
        _display.addLine(n.getNotification());
      }
      _display.display();
      _receiver.CleanNotificationsList(_userID.value());
    } catch (UserNotFoundException e) {
      throw new NoSuchUserException(_userID.value());
    }
  }

}