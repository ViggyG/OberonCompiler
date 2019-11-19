MODULE tstprs;
CONST
x = 65;

TYPE
gram = BOOLEAN;

VAR 
r , y : INTEGER;

PROCEDURE test;
BEGIN
    IF 3 = 2 THEN
    x := 3 + 3;
    END;

    WHILE 1 DO 
    x := 3 * 5 DIV 4;
    END;

END test;

BEGIN
test()
END tstprs.