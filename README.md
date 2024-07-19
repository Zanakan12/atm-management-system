# Atm magement systeme

##history of change
```c
void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            &r->id,
	    &u->id
	    &u->name,
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}
//////////////////////////////////////////////////////////
void saveAccountToFile(FILE *ptr, struct User u, struct Record r)
{
    fprintf(ptr, "%d %d %s %d %d/%d/%d %s %d %.2lf %s\n\n",
            r.id,
            u.id,
            u.name,  // Assumant que name est un tableau de caractères
            r.accountNbr,
            r.deposit.month,
            r.deposit.day,
            r.deposit.year,
            r.country,
            r.phone,
            r.amount,
            r.accountType);
}

```
### Explications et modifications
Changement des -> en . : Puisque u et r sont des structures, utilisez . pour accéder à leurs membres.
Suppression des & : Vous ne devez pas utiliser & pour obtenir l'adresse des champs lorsque vous les passez à fprintf. Vous devez passer directement les valeurs sauf si vous avez besoin de l'adresse pour une raison spécifique, ce qui n'est pas le cas ici.
Formatage de fprintf : Assurez-vous que le format spécifié dans fprintf correspond au type des champs. Par exemple, %d pour les entiers, %s pour les chaînes de caractères, et %.2lf pour les doubles.
