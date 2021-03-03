package m19.app.requests;

import pt.tecnico.po.ui.Command;
import pt.tecnico.po.ui.DialogException;
import pt.tecnico.po.ui.Input;

import m19.LibraryManager;
import m19.User;
import m19.Work;
import m19.Request;

import m19.app.exceptions.WorkNotBorrowedByUserException;

import m19.app.exceptions.NoSuchUserException;
import m19.app.exceptions.NoSuchWorkException;

import m19.exceptions.UserNotFoundException;
import m19.exceptions.WorkNotFoundException;
/**
 * 4.4.2. Return a work.
 */
public class DoReturnWork extends Command<LibraryManager> {

  private Input<Integer> _userID;
  private Input<Integer> _workID;
  private Input<String> _preference;

  /**
   * @param receiver
   */
  public DoReturnWork(LibraryManager receiver) {
    super(Label.RETURN_WORK, receiver);
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

      if(_receiver.isBorrowed(user, work) == false){
        throw new WorkNotBorrowedByUserException(_workID.value(), _userID.value());
      }

      _receiver.updateRequest(user, work);

      int fine = _receiver.getRequest(user, work).getFine();

      _receiver.returnWork(user, work);
      if(fine != 0){
        _display.popup(Message.showFine(_userID.value(),fine+user.getFineValue()));

        _form.clear();

        _preference = _form.addStringInput(Message.requestFinePaymentChoice());
        _form.parse();

        if(_preference.value().equals("s") && user.getFineValue() != 0) user.payFine();
        if(_preference.value().equals("n"))
          user.notPayFine(fine);
    }
    _form.clear();
    }
    catch(UserNotFoundException e){
      throw new NoSuchUserException(_userID.value());
    }catch(WorkNotFoundException e){
      throw new NoSuchWorkException(_workID.value());
    }
  }
}
