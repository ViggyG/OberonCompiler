MODULE tstgcd;
  VAR m, n: INTEGER;

  PROCEDURE gcd(m, n: INTEGER): INTEGER;
  BEGIN
    EXIT;
    WHILE m # n DO
      IF m > n THEN
        m := m - n
      ELSE
        n := n - m
      END
    END;
    RETURN m
  END gcd;

BEGIN
  
END tstgcd.
