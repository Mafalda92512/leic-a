package m19;

import java.io.Serializable;

public class NotificationRequest extends Notification implements Serializable{

  /**
   *
   */
  private static final long serialVersionUID = 4289195632713160154L;

  public NotificationRequest(String notification) {
    super(notification);
  }
  
  @Override
  public String getNotification() {
    return "REQUISIÇÂO: " + super.getNotification();
  }
}
