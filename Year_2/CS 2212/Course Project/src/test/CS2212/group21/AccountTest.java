package CS2212.group21;

import org.json.simple.parser.ParseException;
import org.junit.jupiter.api.Test;

import java.io.IOException;

import static org.junit.jupiter.api.Assertions.*;

class AccountTest {

    Account obj = new Account("src/main/java/CS2212/group21/accounts.json");

    @Test
    void getFavourites() throws IOException, ParseException {
        var test1 = obj.getFavourites("");

        assertEquals("",test1);
    }

    @Test
    void getPassword() throws IOException, ParseException {
        var test2 = obj.getPassword("pw123");

        assertEquals("pw123", test2);

    }

    @Test
    void getUser() throws IOException, ParseException {
        var test3 = obj.getUser("Paul");

        assertEquals("pw123", test3);
    }

    @Test
    void checkAdmin() {

    }

    @Test
    void findUser() {

    }

    @Test
    void checkValidUsername() {

    }

    @Test
    void createAccount() {

    }
}