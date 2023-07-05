package ceng.ceng351.cengvacdb;
import java.sql.*;
import java.util.ArrayList;
import ceng.ceng351.cengvacdb.QueryResult.UserIDuserNameAddressResult;

public class CENGVACDB implements ICENGVACDB{
        static String user = "e2380640";
        static String password = "&FvjosQw6@4c";
        static String database = "db2380640";
        static String host = "144.122.71.121";
        static int port = 8080;
        static Connection connection = null;
    /**
     * Place your initialization code inside if required.
     *
     * <p>
     * This function will be called before all other operations. If your implementation
     * need initialization , necessary operations should be done inside this function. For
     * example, you can set your connection to the database server inside this function.
     */
    @Override
    public void initialize() {
        String url = "jdbc:mysql://" + host + ":" + port + "/" + database;
        try {
            Class.forName("com.mysql.cj.jdbc.Driver");
            connection = DriverManager.getConnection(url, user, password);
        } catch (SQLException | ClassNotFoundException e) {
            e.printStackTrace();
        }
    }

    /**
     * Should create the necessary tables when called.
     *
     * @return the number of tables that are created successfully.
     */
    @Override
    public int createTables(){
        int count = 0;
        String createUser = "create table User (" +
                "userID int," +
                "userName varchar(30)," +
                "age int," +
                "address varchar(150)," +
                "password varchar(30)," +
                "status varchar(15)," +
                "primary key (userID))";
        //
        String createVaccine = "create table Vaccine (" +
                "code int," +
                "vaccinename varchar(30)," +
                "type varchar(30)," +
                "primary key (code))";
        //
        String createVaccination = "create table Vaccination (" +
                "code int," +
                "userID int," +
                "dose int," +
                "vacdate date," +
                "primary key (code,userID,dose)," +
                "foreign key (code) references Vaccine(code)," +
                "foreign key (userID) references User(userID)" +
                "on delete cascade)";
        //
        String createAllergicSideEffect = "create table AllergicSideEffect (" +
                "effectcode int," +
                "effectname varchar(50)," +
                "primary key (effectcode))";
        //
        String createSeen = "create table Seen (" +
                "effectcode int," +
                "code int," +
                "userID int," +
                "date date," +
                "degree varchar(30)," +
                "primary key (effectcode,code,userID)," +
                "foreign key (effectcode) references AllergicSideEffect(effectcode)," +
                "foreign key (code) references Vaccine(code)," +
                "foreign key (userID) references User(userID)" +
                "on delete cascade)";
        try {
            Statement statement = this.connection.createStatement();

            statement.executeUpdate(createUser);
            count++;

            statement.executeUpdate(createVaccine);
            count++;

            statement.executeUpdate(createVaccination);
            count++;

            statement.executeUpdate(createAllergicSideEffect);
            count++;

            statement.executeUpdate(createSeen);
            count++;

            statement.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        //System.out.print(count);
        return count;
    }

    /**
     * Should drop the tables if exists when called.
     *
     * @return the number of tables are dropped successfully.
     */
    @Override
    public int dropTables(){
        int count = 0;
        //DISABLE KEYS;
        String q1 = "SET FOREIGN_KEY_CHECKS = 0";
        String q2 = "SET FOREIGN_KEY_CHECKS=1";
        String queryDropVaccinationTable = "drop table if exists Vaccination";
        String queryDropSeenTable = "drop table if exists Seen";
        String queryDropUserTable = "drop table if exists User";
        String queryDropVaccineTable = "drop table if exists Vaccine";
        String queryDropAllergicSideEffectTable = "drop table if exists AllergicSideEffect";
        try
        {
            Statement statement = this.connection.createStatement();
            statement.executeUpdate(q1);
            statement.executeUpdate(queryDropUserTable);
            count++;

            statement.executeUpdate(queryDropVaccineTable);
            count++;

            statement.executeUpdate(queryDropVaccinationTable);
            count++;

            statement.executeUpdate(queryDropAllergicSideEffectTable);
            count++;

            statement.executeUpdate(queryDropSeenTable);
            count++;
            statement.executeUpdate(q2);
            statement.close();
        } catch(SQLException e)
        {
            e.printStackTrace();
        }
        //System.out.print(count);

        return count;
    }

    /**
     * Should insert an array of User into the database.
     *
     * @return Number of rows inserted successfully.
     */
    @Override
    public int insertUser(User[] users) {
        int count = 0;
        int i = 0;
        while( i < users.length ) {
            String query = "insert into User values (\"" +
                    users[i].getUserID() + "\",\"" +
                    users[i].getUserName() + "\",\"" +
                    users[i].getAge() + "\",\"" +
                    users[i].getAddress() + "\",\"" +
                    users[i].getPassword() + "\",\"" +
                    users[i].getStatus() + "\")";
            try {
                Statement st = connection.createStatement();
                st.executeUpdate(query);
                count++ ;
                st.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
            i++;

        }
        return count;
    }
    

    /**
     * Should insert an array of AllergicSideEffect into the database.
     *
     * @return Number of rows inserted successfully.
     */
    @Override
    public int insertAllergicSideEffect(AllergicSideEffect[] sideEffects){
        int count = 0;
        int i = 0;
        while( i < sideEffects.length ) {
            String query = "insert into AllergicSideEffect values (\"" +
                    sideEffects[i].getEffectCode() + "\",\"" +
                    sideEffects[i].getEffectName() + "\")";
            try {
                Statement st = connection.createStatement();
                st.executeUpdate(query);
                count++ ;
                st.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
            i++;

        }
        return count;
    }

    /**
     * Should insert an array of Vaccine into the database.
     *
     * @return Number of rows inserted successfully.
     */
    @Override
    public int insertVaccine(Vaccine[] vaccines){
        int count = 0;
        int i = 0;
        while( i < vaccines.length ) {
            String query = "insert into Vaccine values (\"" +
                    vaccines[i].getCode() + "\",\"" +
                    vaccines[i].getVaccineName() + "\",\"" +
                    vaccines[i].getType() + "\")";
            try {
                Statement st = connection.createStatement();
                st.executeUpdate(query);
                count++ ;
                st.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
            i++;

        }
        return count;
    }

    /**
     * Should insert an array of Vaccination into the database.
     *
     * @return Number of rows inserted successfully.
     */
    @Override
    public int insertVaccination(Vaccination[] vaccinations){
        int count = 0;
        int i = 0;
        while( i < vaccinations.length ) {
            String query = "insert into Vaccination values (\"" +
                    vaccinations[i].getCode() + "\",\"" +
                    vaccinations[i].getUserID() + "\",\"" +
                    vaccinations[i].getDose() + "\",\"" +
                    vaccinations[i].getVacdate() + "\")";
            try {
                Statement st = connection.createStatement();
                st.executeUpdate(query);
                count++ ;
                st.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
            i++;

        }
        return count;
    }

    /**
     * Should insert an array of Seen into the database.
     *
     * @return Number of rows inserted successfully.
     */
    @Override
    public int insertSeen(Seen[] seens){
        int count = 0;
        int i = 0;
        while( i < seens.length ) {
            String query = "insert into Seen values (\"" +
                    seens[i].getEffectcode() + "\",\"" +
                    seens[i].getCode() + "\",\"" +
                    seens[i].getUserID() + "\",\"" +
                    seens[i].getDate() + "\",\"" +
                    seens[i].getDegree() + "\")";
            try {
                Statement st = connection.createStatement();
                st.executeUpdate(query);
                count++ ;
                st.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
            i++;

        }
        return count;
    }

    /**
     * Should return vaccines that have not been applied to any user
     *
     * @return Vaccine[]
     */
    @Override
    public Vaccine[] getVaccinesNotAppliedAnyUser(){
        ArrayList<Vaccine> lst = new ArrayList<Vaccine>();
        Vaccine r[];
        Vaccine r_new;
        ResultSet rs;
        String Query = "Select v.code,v.vaccinename,v.type " +
                "From Vaccine v " +
                "Where v.code NOT IN (Select vac.code " +
                                    "from Vaccination vac) " +
                                    "order by v.code;";
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(Query);

            while( rs.next() ) {
                int r_code= rs.getInt("code");
                String r_vaccinename = rs.getString("vaccinename");
                String r_type = rs.getString("type");
                r_new = new Vaccine(r_code, r_vaccinename, r_type);
                lst.add(r_new) ;

            }
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        r = new Vaccine[lst.size()];
        r = lst.toArray(r);
        return r;
    }

    /**
     * Should return the users who have been vaccinated for two doses since the
     * given date
     *
     * @return UserIDuserNameAddressResult[]
     */
    @Override
    public QueryResult.UserIDuserNameAddressResult[] getVaccinatedUsersforTwoDosesByDate(String vacdate){
        ResultSet rs;
        ArrayList<QueryResult.UserIDuserNameAddressResult> lst = new ArrayList<QueryResult.UserIDuserNameAddressResult>();
        QueryResult.UserIDuserNameAddressResult r[];
        QueryResult.UserIDuserNameAddressResult r_new;
        String Query = "Select userID,username,address " +
                "From User u " +
                "Where u.userid NOT IN (Select v.userID " +
                "                       From Vaccination v " +
                "                       Where u.userID = v.userID and v.dose > 2) and " +
                "                        Exists (Select v2.userID " +
                "                        From Vaccination v2 " +
                "                        Where v2.userID = u.userID and v2.dose = 1 and ' " + vacdate + " ' < v2.vacdate) " +
                "                        and " +
                "                        Exists (Select v3.userID " +
                "                        From Vaccination v3 " +
                "                        Where u.userID = v3.userID and v3.dose = 2 and ' " + vacdate + " ' < v3.vacdate) " +
                "                        order by u.userID; ";
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(Query);

            while( rs.next() ) {
                //System.out.print("asd\n");
                String userid= rs.getString("userid");
                String username = rs.getString("username");
                String address = rs.getString("address");
                r_new = new QueryResult.UserIDuserNameAddressResult(userid, username, address);
                lst.add(r_new) ;
            }
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        r = new QueryResult.UserIDuserNameAddressResult[lst.size()];
        r = lst.toArray(r);
        return r;
    }

    /**
     * List of the two recent vaccines applied to any user that
     * do not contain ‘vac’ in their name
     *
     * @return Vaccine[]
     */
    @Override
    public Vaccine[] getTwoRecentVaccinesDoNotContainVac(){

        ArrayList<Vaccine> lst = new ArrayList<Vaccine>();
        Vaccine r[];
        Vaccine r_new;
        ResultSet rs;
        String Query = "Select Distinct v2.code,v2.vaccinename,v2.type,v.vacdate " +
                "From Vaccination v,Vaccine v2 " +
                "Where v.code = v2.code and v2.vaccinename NOT LIKE '%vac%'"+
                "order by v.vacdate DESC;";
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(Query);
            while( rs.next() ) {
                int r_code= rs.getInt("code");
                String r_vaccinename = rs.getString("vaccinename");
                String r_type = rs.getString("type");
                r_new = new Vaccine(r_code, r_vaccinename, r_type);
                lst.add(r_new) ;
            }
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        r = new Vaccine[lst.size()];
        r = lst.toArray(r);
        Vaccine r2[] = new Vaccine[2];
        r2[0] = r[0];
        int i = 1;
        /*int j = 0;
        while(j < r.length){
            System.out.print(r[j].getCode() + "\n");
            j++;
        }*/
        while(true){
            if(r2[0].getCode() != r[i].getCode()){
                r2[1] = r[i];
                break;
            }
            else{
                i++;
            }
        }
        return r2;
    }

    /**
     * Should return users who have had at least two doses of vaccine and
     * have experienced at most one side effect
     *
     * @return UserIDuserNameAddressResult[]
     */
    @Override
    public QueryResult.UserIDuserNameAddressResult[] getUsersAtHasLeastTwoDoseAtMostOneSideEffect(){
        String Query = "Select Distinct u.userid,u.userName,u.address " +
                "From Vaccination v,User u " +
                "Where u.userid = v.userid and v.dose > 1 and NOT EXISTS (Select * " +
                "                                                           From Seen s1,Seen s2 " +
                "                                                           Where u.userid = s1.userid and s1.userid = s2.userid " +
                "                                                           and s1.effectcode <> s2.effectcode) " +
                "order by u.userid; ";
        ResultSet rs;
        ArrayList<QueryResult.UserIDuserNameAddressResult> lst = new ArrayList<QueryResult.UserIDuserNameAddressResult>();
        QueryResult.UserIDuserNameAddressResult r[];
        QueryResult.UserIDuserNameAddressResult r_new;
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(Query);

            while( rs.next() ) {
                String userid= rs.getString("userid");
                String username = rs.getString("username");
                String address = rs.getString("address");
                r_new = new QueryResult.UserIDuserNameAddressResult(userid, username, address);
                lst.add(r_new) ;
            }
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        r = new QueryResult.UserIDuserNameAddressResult[lst.size()];
        r = lst.toArray(r);
        return r;
    }
    /**
     * Should return users who have been vaccinated with all vaccines that
     * can cause the given side effect
     *
     * @return UserIDuserNameAddressResult[]
     */
    @Override
    public QueryResult.UserIDuserNameAddressResult[] getVaccinatedUsersWithAllVaccinesCanCauseGivenSideEffect(String effectname){
        String Query = "Select Distinct u.userid,u.userName,u.address " +
                "From User u " +
                "Where NOT EXISTS (Select * " +
                "From (Select Distinct s.code AS code " +
                "From Seen s,AllergicSideEffect a " +
                "Where s.effectcode = a.effectcode and a.effectname = \"" + effectname + "\") AS temp " +
                "Where temp.code NOT IN (Select Distinct v.code AS code " +
                "From Vaccination v " +
                "Where v.userid = u.userid )) " +
                "order by u.userid;";
        ResultSet rs;
        ArrayList<QueryResult.UserIDuserNameAddressResult> lst = new ArrayList<QueryResult.UserIDuserNameAddressResult>();
        QueryResult.UserIDuserNameAddressResult r[];
        QueryResult.UserIDuserNameAddressResult r_new;
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(Query);

            while( rs.next() ) {
                String userid= rs.getString("userid");
                String username = rs.getString("username");
                String address = rs.getString("address");
                r_new = new QueryResult.UserIDuserNameAddressResult(userid, username, address);
                lst.add(r_new) ;
            }
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        r = new QueryResult.UserIDuserNameAddressResult[lst.size()];
        r = lst.toArray(r);
        return r;
    }

    /**
     * Should return users who have been vaccinated with at least two different
     * types of vaccines within the given time interval
     *
     * @return UserIDuserNameAddressResult[]
     */
    @Override
    public QueryResult.UserIDuserNameAddressResult[]  getUsersWithAtLeastTwoDifferentVaccineTypeByGivenInterval(String startdate, String enddate){
        String Query = "Select Distinct u.userid,u.userName,u.address " +
                "From User u,Vaccination v1,Vaccination v2 " +
                "Where u.userid = v1.userid and u.userid = v2.userid and " +
                "v1.code <> v2.code and v1.vacdate >= \"" + startdate + "\" and v1.vacdate <= \"" + enddate + "\" and  " +
                "v2.vacdate >= \"" + startdate + "\" and v2.vacdate <= \"" + enddate + "\"" +
                "order by u.userid;";
        ResultSet rs;
        ArrayList<QueryResult.UserIDuserNameAddressResult> lst = new ArrayList<QueryResult.UserIDuserNameAddressResult>();
        QueryResult.UserIDuserNameAddressResult r[];
        QueryResult.UserIDuserNameAddressResult r_new;
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(Query);

            while( rs.next() ) {
                String userid= rs.getString("userid");
                String username = rs.getString("username");
                String address = rs.getString("address");
                r_new = new QueryResult.UserIDuserNameAddressResult(userid, username, address);
                lst.add(r_new) ;
            }
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        r = new QueryResult.UserIDuserNameAddressResult[lst.size()];
        r = lst.toArray(r);
        return r;
    }

    /**
     * Should return side effects experienced by users who have two doses of
     * vaccine with less than 20 days between doses
     *
     * @return AllergicSideEffect[]
     */
    @Override
    public AllergicSideEffect[] getSideEffectsOfUserWhoHaveTwoDosesInLessThanTwentyDays(){
        String Query = "Select Distinct a.effectcode,a.effectname " +
                "From Vaccination v1,Vaccination v2,AllergicSideEffect a,Seen s " +
                "Where v1.code = v2.code and v1.userid = v2.userid and v1.dose = 1 and v2.dose = 2 and " +
                " DATEDIFF(v2.vacdate,v1.vacdate) < 20 and v1.code = s.code and s.userid = v1.userid and s.effectcode = a.effectcode " +
                "order by a.effectcode;";
        ResultSet rs;
        ArrayList<AllergicSideEffect> lst = new ArrayList<AllergicSideEffect>();
        AllergicSideEffect r[];
        AllergicSideEffect r_new;
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(Query);

            while( rs.next() ) {
                int effectcode= rs.getInt("effectcode");
                String effectname = rs.getString("effectname");
                r_new = new AllergicSideEffect(effectcode,effectname);
                lst.add(r_new) ;
            }
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        r = new AllergicSideEffect[lst.size()];
        r = lst.toArray(r);
        return r;
    }

    /**
     * Calculate the average number of doses of vaccinated users over the age of 65
     *
     * @return average
     */
    @Override
    public double averageNumberofDosesofVaccinatedUserOverSixtyFiveYearsOld(){
        String Query = "Select avg(mcount) AS rt " +
                "From (Select u.userid,max(v.dose) AS mcount " +
                "From User u,Vaccination v " +
                "Where u.age > 65 and v.userid = u.userid " +
                "group by u.userid) AS temp;";
        ResultSet rs;
        double rt = 0;
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(Query);
            while( rs.next() ) {
                double mcount= rs.getDouble("rt");
                rt = mcount;
            }
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return rt;
    }

    /**
     * Update his/her status to “eligible” after 120 days have
     * passed since the user was vaccinated last time
     *
     * @return the row count for SQL Data Manipulation Language (DML) statements
     */
    @Override
    public int updateStatusToEligible(String givendate){
        int count = 0 ;
        String e1 = "Eligible";
        String e2 = "Not_Eligible";
        String Query = "Update User " +
                "Set status = \"" + e1 +"\" " +
                "Where status = \"" + e2 + "\" and userid IN (Select v.userid " +
                "                                           From Vaccination v " +
                "                                           Where v.userid = userid " +
                "                                           group by v.userid " +
                "                                           having DATEDIFF( \"" +givendate+ "\" ,max(v.vacdate)) > 120);";

        try {
            Statement st = connection.createStatement();
            count = st.executeUpdate(Query);

            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return count;
    }

    /**
     * Delete the vaccine for the given vaccineName
     *
     * @return Vaccine
     */
    @Override
    public Vaccine deleteVaccine(String vaccineName){
        String Query = "Select * " +
                "From Vaccine v " +
                "Where v.vaccinename = \"" + vaccineName + "\" ;";
        ResultSet rs;
        Vaccine v = null;
        //System.out.print(vaccineName);
        try {
            Statement st = connection.createStatement();
            rs = st.executeQuery(Query);
            rs.next();
            //System.out.print("asd");
            int code= rs.getInt("code");
            String vaccinename= rs.getString("vaccinename");
            String type= rs.getString("type");
            v = new Vaccine(code,vaccinename,type);

            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        Query = "delete " +
                "from Vaccine " +
                "where vaccinename = \"" + vaccineName + "\" ;";
        try {
            Statement st = connection.createStatement();
            st.executeUpdate(Query);
            st.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return v;
    }

}