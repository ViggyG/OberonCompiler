MODULE tstgcd;
  IMPORT In, Out;
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
  Out.String( 'm = ');
  In.Int( m);
  Out.String( "n = ");
  In.Int( n);
  Out.String( "GCD = ");
  Out.Int( gcd( m, n), 2);
  Out.Ln;
END.. tstgcd.
