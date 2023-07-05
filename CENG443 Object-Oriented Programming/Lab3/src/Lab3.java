import java.text.ParseException;
import java.util.*;
import java.util.stream.*;
import java.io.*;
import java.util.function.*;
import java.text.SimpleDateFormat;

public class Lab3 {
    private static SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd"); // years first

    public static void main(String[] args) throws FileNotFoundException {
        String csv = args[0];
        int questionNumber = Integer.parseInt(args[1]);
        BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream(new File(csv))));

        Function<String, Customer> mapToCustomer = (line) -> {
            String[] p = line.split(",");
            String[] q = new String[7];  // Create a new array with 7 elements
            System.arraycopy(p, 0, q, 0, p.length);  // Copy elements from p to q
            return new Customer(Integer.parseInt(q[0]), parseDate(q[1]), parseFloat(q[2]), parseFloat(q[3]), parseFloat(q[4]), parseFloat(q[5]), parseFloat(q[6]));
        };

        List<Customer> crs = br.lines().skip(1).map(mapToCustomer).collect(Collectors.toList());
        if (questionNumber == 1) {
            Calendar calendar = Calendar.getInstance();
            calendar.set(Calendar.DAY_OF_MONTH, 15);
            Date beginningDate;
            Date endingDate;

            try {
                beginningDate = dateFormat.parse("1980-01-01");
                endingDate = dateFormat.parse("1990-01-01");
            } catch (ParseException e) {
                throw new RuntimeException("Error", e);
            }
            Map<String, Long> productCount = new HashMap<>();
            productCount.put("A", crs.stream().filter(c -> c.date.before(endingDate) && c.date.after(beginningDate) && c.A != null)
                    .filter(customer -> {
                        calendar.setTime(customer.date);
                        return calendar.get(Calendar.DAY_OF_MONTH) == 15;  })
                    .count());
            productCount.put("B", crs.stream().filter(c -> c.date.before(endingDate) && c.date.after(beginningDate) && c.B != null)
                    .filter(customer -> {
                        calendar.setTime(customer.date);
                        return calendar.get(Calendar.DAY_OF_MONTH) == 15;  })
                    .count());
            productCount.put("C", crs.stream().filter(c -> c.date.before(endingDate) && c.date.after(beginningDate) && c.C != null)
                    .filter(customer -> {
                        calendar.setTime(customer.date);
                        return calendar.get(Calendar.DAY_OF_MONTH) == 15;  })
                    .count());
            productCount.put("D", crs.stream().filter(c -> c.date.before(endingDate) && c.date.after(beginningDate) && c.D != null)
                    .filter(customer -> {
                        calendar.setTime(customer.date);
                        return calendar.get(Calendar.DAY_OF_MONTH) == 15;  })
                    .count());
            productCount.put("E", crs.stream().filter(c -> c.date.before(endingDate) && c.date.after(beginningDate) && c.E != null)
                    .filter(customer -> {
                        calendar.setTime(customer.date);
                        return calendar.get(Calendar.DAY_OF_MONTH) == 15;  })
                    .count());

            Map.Entry<String, Long> mostVital = productCount.entrySet().stream()
                    .max(Map.Entry.comparingByValue())
                    .orElse(null);

            System.out.println(mostVital.getKey());

        } else if (questionNumber == 2) {
            int count = (int) crs.stream()
                    .filter(customer -> {
                        int productCount = (customer.A != null ? 1 : 0) + (customer.B != null ? 1 : 0) + (customer.C != null ? 1 : 0) + (customer.D != null ? 1 : 0) + (customer.E != null ? 1 : 0);
                        return productCount >= 3;
                    })
                    .filter(customer -> customer.D != null || customer.A != null)
                    .count();
            System.out.println(count);

        }  else if (questionNumber == 3) {
            Date startDate;
            try{
                startDate = dateFormat.parse("1995-01-01");
            }catch (ParseException e) {
                throw new RuntimeException("Error", e);
            }
            int count = (int) crs.stream()
                    .filter(customer -> !customer.date.before(startDate) && customer.age >= 55 )
                    .flatMap(customer -> Stream.of(customer.A, customer.B, customer.C, customer.D, customer.E))
                    .filter(Objects::nonNull)
                    .count();

            System.out.println(count);
        } else if (questionNumber == 4) {
            Map<Integer, Long> count = crs.stream()
                    .filter(c -> c.B != null)
                    .collect(Collectors.groupingBy(
                            customer -> customer.date.getYear(), Collectors.counting()
                    ));

            Map.Entry<Integer, Long> maxYear = Collections.max(count.entrySet(), Map.Entry.comparingByValue());
            Map.Entry<Integer, Long> minYear = Collections.min(count.entrySet(), Map.Entry.comparingByValue());

            System.out.println(minYear.getKey() + 1900); // it has - 1900 inside the getyear function so add 1900
            System.out.println(maxYear.getKey() + 1900); // it has - 1900 inside the getyear function so add 1900
        } else {
            // empty
        }
    }
    private static Date parseDate(String str) {
        try {
            return dateFormat.parse(str);
        } catch (ParseException e) {
            return null;
        }
    }

    private static Float parseFloat(String str) {
        if (str == null || str.trim().isEmpty()) {
            return null;
        }
        try {
            return Float.parseFloat(str);
        } catch (NumberFormatException e) {
            return null;
        }
    }

}

class Customer {
    Integer age;
    Date date;
    Float A;
    Float B;
    Float C;
    Float D;
    Float E;
    Customer(int age, Date date, Float a, Float b, Float c, Float d, Float e) {
        this.age = age;
        this.date = date;
        this.A = a;
        this.B = b;
        this.C = c;
        this.D = d;
        this.E = e;
    }
}
