login:=xsleza26

.PHONY: doc
doc:
	cd doc && make

.PHONY: submit
submit:
	cd doc && make
	cp doc/doc.pdf $(login).pdf
	zip -r $(login).zip .settings Debug Includes Project_Settings Sources \
		.cproject .cwGeneratedFileSetLog .project $(login).pdf

.PHONY: clean
clean:
	cd doc && make clean
	rm -r $(login).pdf $(login).zip
