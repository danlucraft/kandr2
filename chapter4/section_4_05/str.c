int streq(char* a, char* b)
{
	int i = 0;
	while (a[i] != '\0') {
		if (a[i] != b[i])
			return 0;
		i++;
	}
	if (b[i] != '\0')
		return 0;
	return 1;
}

