package m19;

import java.io.Serializable;

public class NotificationReturn extends Notification implements Serializable{

  /**
   *
   */
  private static final long serialVersionUID = -18668542143500019L;

  public NotificationReturn(String notification) {
    super(notification);
  }

  @Override
  public String getNotification() {
    return "ENTREGA: " + super.getNotification();
  }
}
