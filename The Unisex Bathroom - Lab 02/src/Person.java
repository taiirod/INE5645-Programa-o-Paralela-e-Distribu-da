import java.util.Random;
import java.util.concurrent.TimeUnit;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Person implements Runnable {

    // Nome da pessoa
    private final String name;
    // Sexo da pessoa
    private final Sex sex;
    // Banheiro
    private final Bathroom bathroom;
    // Pode sair?
    private boolean canLeave;
    // Precisa ir ao banheiro
    private boolean needBathroom;

    public Person(String name, Sex sex, Bathroom bathroom) {
        this.name = name;
        this.sex = sex;
        this.bathroom = bathroom;
        this.canLeave = false;
        this.needBathroom = true;
    }

    public void useBathroom() {
        // Entrar no banheiro
        this.bathroom.addUser(this);

        if (this.bathroom.isInTheBathroom(this)) {
            try {
                // Tempo gasto no banheiro
                TimeUnit.SECONDS.sleep((new Random()).nextInt(1) + 1);
                this.canLeave = true;
                System.out.println(getName() + " - Terminou");
            } catch (InterruptedException ex) {
                Logger.getLogger(Person.class.getName())
                        .log(Level.SEVERE, null, ex);
            }
        }
    }

    // Sair do banheiro
    public void leaveBathroom() {
        // Sair do banheiro
        this.bathroom.removeUser(this);
        this.canLeave = false;
        this.needBathroom = false;
    }

    @Override
    public void run() {
        System.out.println(this.getName());
        // Enquanto pessoa da vez precisar ir no banheiro
        while (this.needBathroom) {
            try {
                Thread.sleep(500);
            } catch (InterruptedException ex) {
                Logger.getLogger(Person.class.getName()).log(Level.SEVERE, null, ex);
            }
            // Se elas querem usar o banheiro
            if ((this.bathroom.getCurrentSex().equals(this.getSex())
                    || this.bathroom.getCurrentSex().equals(Sex.NONE))
                    && !this.bathroom.isFull()
                    && !this.bathroom.isInTheBathroom(this)) {
                this.useBathroom();
            }
            // Se elas querem sair
            if (this.canLeave) {
                this.leaveBathroom();
            }
        }
    }

    // Busca nome da pessoa
    public String getName() {
        return this.name;
    }

    // Busca sexo da pessoa
    public Sex getSex() {
        return this.sex;
    }

    @Override
    public String toString() {
        return "Person{" + "name = " + this.name + ", sex = " + this.sex + '}';
    }
}
