package m19.app.requests;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;

import m19.User;
import m19.Work;
import m19.LibraryManager;

import m19.app.exceptions.NoSuchUserException;
import m19.app.exceptions.NoSuchWorkException;
import m19.app.exceptions.RuleFailedException;

import m19.exceptions.UserNotFoundException;
import m19.exceptions.WorkNotFoundException;
/**
 * 4.4.1. Request work.
 */
public class DoRequestWork extends Command<LibraryManager> {

  private Input<Integer> _userID;
  private Input<Integer> _workID;
  private Input<String> _preference;

  /**
   * @param receiver
   */
  public DoRequestWork(LibraryManager receiver) {
    super(Label.REQUEST_WORK, receiver);
  }

  /** @see pt.tecnico.po.ui.Command#execute() */
  @Override
  public final void execute() throws DialogException {
    try{
      _form.clear();
      
      _userID = _form.addIntegerInput(Message.requestUserId());
      _workID = _form.addIntegerInput(Message.requestWorkId());

      _form.parse();

        User user = _receiver.showUser(_userID.value());
        Work work = _receiver.displayWork(_workID.value());

        int _regra = _receiver.requestWork(user, work);

        if( _regra ==0 ){
          int returnTime = _receiver.calcTime(user, work.getCopies());
        _display.popup(Message.workReturnDay(_workID.value(), returnTime));
        }
        else if(_regra == 3){
              _form.clear();
              _preference = _form.addStringInput(Message.requestReturnNotificationPreference());
              _form.parse();
          if (_preference.value().equals("s")) {
            _receiver.requestsNotification(_workID.value(), user);
          }
        }
      else{
        throw new RuleFailedException(_userID.value(), _workID.value(), _regra);
      }
      _form.clear();
    }
    catch(UserNotFoundException e){
      throw new NoSuchUserException(_userID.value());
    }
    catch(WorkNotFoundException e){
      throw new NoSuchWorkException(_workID.value());
    }
  }
}
