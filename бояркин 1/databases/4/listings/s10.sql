CREATE PROCEDURE S10 AS
BEGIN
	UPDATE DRUG SET DRUG.DRUG_SHELF_LIFE = DRUG.DRUG_SHELF_LIFE + 1;
	COMMIT;
END;