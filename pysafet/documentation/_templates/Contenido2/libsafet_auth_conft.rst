	
:command:`auth.conf` (Archivo de autenticación y autorización)
-----------------------------------------------------------------

	En este archivo (auth.conf) se especifican los valores de autorización, autenticación y conexión a las base de datos. los Tipo de gestor de base de datos que admitidos

	**database.driver.1 = psql** 

	- Psql : `Postgresql`_.

.. _Postgresql: http://www.postgresql.org/

	- Sqlite : `sqlite3`_.

.. _sqlite3: http://www.sqlite.org/
	
 :command:`1.- [Database]:`

	En esta sección se configura la fuentes de datos. Una fuente de datos esta asociada a una conexión con una base de datos relacional. Es posible tener varias fuentes de datos, cada una representada por un indice. 

 ::


	[Database] 

	# Información de configuración de fuentes de datos. 
	# Una fuente de datos esta asociada a una conexión con una base 
	# de datos relacional. Es posible tener varias fuentes de datos, cada 
	# una representada por un indice. 

	# Especifica el numero de fuentes de datos diferentes 
	database.datasources = 1 				------------------------->Ejemplo: elemento.1 = xxxx, elemento.2 = xxxx 

 :Note: Esté elemento :command:`(database.datasources)` me indica el número de base de datos, de aquí en adelante todos los elementos que se  requiera estará identificado por el indice asociado a una base de datos. 


 ::

	# Para realizar la conexión de base de datos asociada a una fuente de datos 
	# se requieren varios elementos: 

	# Nombre de la fuente de datos 1 
	database.sourcename.1 = nombre_fuente_datos_1 

	# Especifica el controlador asociado a la fuente de datos 
	# Tipo de gestor de base de datos admitidos
	
	database.driver.1 = controlador_fuente_datos_1       ------------------>Ejemplo:  database.driver.1 = psql



	# Especifica si la fuente de datos esta activa o no 
	database.actived.1 = on 

	# Especifica el nombre de host para la fuente de datos 
	database.host.1 = host_base_de_datos                  ------------------>Ejemplo: 127.0.0.0

	# Especifica el nombre de la base de datos para la fuente de datos 

	database.db.1 = nombre_database .1

	# Especifica el puerto de conexion para la base de datos 
	database.port.1 = puerto_database.1

	# Especifica el nombre de usuario para la fuente de datos 
	database.user.1 = usuario_database.1

	# Especifica la contrasena de acceso para la fuente de datos 
	database.password.1 = contrasena_database.1 



 :command:`2.- [Roles]:`
	En esta sección se define los roles que se van a utilizar en el sistema, la definición de un rol comprende dos :command:`(2)` elementos:

 - :command:`roles.name.n =` nombre del rol n.
 - :command:`roles.description.n =` descripción del rol n.


 ::

	[Roles] 

	roles.name.1 = Administrador 
	roles.description.1 = usuario(s) que administra el sistema 

	roles.name.2 = rol_1
	roles.description.2 = descripción del rol  --------------------> Ejemplo: Acciones asociadas al grupo o al usuario con el rol_1 

	roles.name.3 = rol_2
	roles.description.3 = descripción del rol  --------------------> Ejemplo: Acciones asociadas al grupo o al usuario con el rol_2
	.
	.
	.

	roles.name.n = rol_n
	roles.description.3 = descripción del rol  --------------------> Ejemplo: Acciones asociadas al grupo o al usuario con el rol_n



 :command:`3.- [Auth]:`
	En esta sección se agrega los usuarios del sistemas, la definición de un usuario comprende cuatro :command:`(4)` elementos:

 - :command:`auth.account.n =` nombre del usuario n.
 - :command:`auth.pass.n =` contrañena del usuario n cifrada en md5.
 - :command:`auth.realname =` nombre apellido y correo del usuario n separada por espacio. 
 - :command:`auth..role.n =` rol del usuario n, este rol debe esta definido en el sección [Roles].


 ::

	[Auth] 

	auth.account.1 = usuario 1            --------------------------------------> Ejemplo: admin 
	auth.pass.1 = 0f885ebbc5a6c77dac0c319a7411f6039496f06f        --------------> Nota: contraseña del admin en md5 
	auth.realname.1 = Nombre_apellido_usuario1  correo_usuario1   --------------> Ejemplo: admin@mail.com 
	auth.role.1 = rol del usuario                                 --------------> Ejemplo: Administrador 

	auth.account.2 = usuario1
	auth.pass.2 = 22cd2d1c596f4091e248aff0e4aa0d47c84b2b36 
	auth.realname.2 = Nombre_apellido_usuario1 usuario1@mail.com 
	auth.role.2 = rol del usuario 

	.
	.
	.

	auth.account.n = usuarion
	auth.pass.n = ddc6fdd484d5a71b9619beb8b19a7ea06980d8ff 
	auth.realname.n = Nombre_apellido_usuarion usuarion@mail.com 
	auth.role.n = Desarrollador 


 :command:`4.- [Permises]:`
 
	En esta sección se van a definir los permisos de los usuarios en función de los roles y acciones que puedan ejecutar en el sistema, la definición de cada permisos comprende cuatro :command:`(4)` elementos: 

 - :command:`permises.operation.n =`  nombre de la operación n a ejecutar (estas acción debe estar definida en el archivo deftrac.xml que mas adelante se explicara). 

 - :command:`permises.accounts.n =` nombre del o los usuario(s) que ejecutara(n) la operación (esta usuario debe estar definido en la sección [Auth]). Si son varios usuarios deben estar separados por punto y como (;) :command:`Ejemplo:` permises.accounts.n = usuario1;usuario2;usuarion.


 - :command:`permises.types.3 =` tipo de acción a ejecutar, entre las acciones tenemos: read,execute y modify deben ir separadas por punto y como (;) :command:`Ejemplo:` permises.types.3 = read;execute;modify.

 - :command:`permises.roles.4 =` rol que puede ejecutar esta acción, de esta manera se puede indicar a un grupo de usuarios que tenga un rol en especifico ejecutar la acción sin la necesidad de especificarlo en el elemento :command:`permises.accounts.n`, los roles debe estar definidos en la sección :command:`[Roles]`.



 :: 


	[Permises] 

	permises.operation.1 = operacion_1
	permises.accounts.1 = usuario1;admin 
	permises.types.1 = read;execute;modify 
	permises.roles.1 = Administrador;rol_1;rol_2

	permises.operation.2 = operación_2
	permises.accounts.2 = admin 
	permises.types.2 = read;execute;modify 
	permises.roles.2 = Administrador 
	.
	.
	.
	permises.operation.n = operacion_n 
	permises.accounts.n = admin 
	permises.types.n = read;execute;modify 
	permises.roles.3 = Administrador;rol_1;rol_2

	# Operaciones de Consulta, Lista de datos y gráficos

	permises.operation.24 = Listar_datos
	permises.accounts.24 = admin
	permises.types.24 = read;execute;modify
	permises.roles.24 = Desarrollador;Administrador


	permises.operation.25 = Listar_datos_con_autofiltro
	permises.accounts.25 = admin
	permises.types.25 = read;execute;modify
	permises.roles.25 = Administrador


	# Viñetas

	permises.operation.31 = Formulario
	permises.accounts.31 = admin
	permises.types.31 = read;execute;modify
	permises.roles.31 = Administrador;Desarrollador


	permises.operation.32 = Consulta
	permises.accounts.32 = admin
	permises.types.32 = read;execute;modify
	permises.roles.32 = Administrador;Desarrollador



