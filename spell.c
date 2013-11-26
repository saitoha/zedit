#include "z.h"

#if SPELL
#include <aspell.h>
#include <dlfcn.h>

static int zsymerr;

/* This is just for checkpatch... the if could be part of the macro. */
static void *zdlsym(void *dl, char *sym)
{
	void *p = dlsym(dl, sym);
	if (!p)
		zsymerr++;
	return p;
}

#define ZDLSYM(A, B) A(*B)() = zdlsym(dl, #B)

void Zspell_word(void)
{
	void *dl = dlopen("libaspell.so", RTLD_LAZY);
	if (!dl) {
		error("You do not have libaspell.so");
		return;
	}

	zsymerr = 0;
	ZDLSYM(AspellConfig *, new_aspell_config);
	ZDLSYM(void , aspell_config_replace);
	ZDLSYM(AspellCanHaveError *, new_aspell_speller);
	ZDLSYM(int , aspell_error_number);
	ZDLSYM(AspellSpeller *, to_aspell_speller);
	ZDLSYM(int , aspell_speller_check);
	ZDLSYM(const AspellWordList *, aspell_speller_suggest);
	ZDLSYM(AspellStringEnumeration *, aspell_word_list_elements);
	ZDLSYM(const char *, aspell_string_enumeration_next);
	ZDLSYM(void , delete_aspell_string_enumeration);
	ZDLSYM(void , delete_aspell_speller);
	ZDLSYM(void , delete_aspell_config);
	if (zsymerr) {
		error("You have an incomplete libaspell.so");
		goto done;
	}

	AspellConfig *config = new_aspell_config();
	if (!config) {
		error("Aspell failed");
		goto done;
	}

	aspell_config_replace(config, "lang", "en_US");

	AspellCanHaveError *possible_err = new_aspell_speller(config);
	if (aspell_error_number(possible_err)) {
		error("Aspell failed");
		goto done;
	}
	AspellSpeller *speller = to_aspell_speller(possible_err);

	char word[STRMAX];
	getbword(word, sizeof(word), bisword);
	int size = strlen(word);
	int correct = aspell_speller_check(speller, word, size);

	if (correct)
		putpaw("%s correct", word);
	else {
		int n, i = 0, len = Colmax;

		const AspellWordList *suggestions =
			aspell_speller_suggest(speller, word, size);
		AspellStringEnumeration *elements =
			aspell_word_list_elements(suggestions);
		const char *s;
		while ((s = aspell_string_enumeration_next(elements)) &&
		       len > 0) {
			if (i == 0)
				n = snprintf(PawStr + i, len, "%s", s);
			else
				n = snprintf(PawStr + i, len, " %s", s);
			i += n;
			len -= n;
		}
		delete_aspell_string_enumeration(elements);

		putpaw(PawStr);
	}

	delete_aspell_speller(speller);
	delete_aspell_config(config);

done:
	dlclose(dl);
}
#else
void Zspell_word(void) { tbell(); }
#endif
