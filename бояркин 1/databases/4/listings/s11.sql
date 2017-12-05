CREATE PROCEDURE S11 AS
BEGIN
	INSERT INTO DRUG (ID_DRUG, DRUG_NAME, DRUG_SHELF_LIFE, DRUG_CURRENT_COST, DRUG_RECIPE_NEEDED)
	VALUES (100001, '�������', 9999999.0, 9999999.0, 1);
	COMMIT;

	DELETE FROM DRUG
	WHERE DRUG.DRUG_CURRENT_COST = (SELECT MAX(DRUG.DRUG_CURRENT_COST) FROM DRUG);
	COMMIT;
END;
