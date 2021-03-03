package m19;

import java.io.Serializable;

public class Notification implements Serializable{
    /**
     *
     */
    private static final long serialVersionUID = 6593527707960217594L;
    private String _notification;

    public Notification(String notification) {
        _notification = notification;
    }

    public String getNotification() {
        return _notification;
    }

}