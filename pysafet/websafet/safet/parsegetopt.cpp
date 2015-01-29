
bool MainWindow::parse(int &argc, char **argv) {
       int c;
       int verbose_flag = 0;
       qDebug("argv[0]: |%s|", argv[0]);
       qDebug("argv[1]: |%s|", argv[1]);
        parsed = true;
       while (1)
         {
             struct option long_options[] =
             {
               /* These options set a flag. */
               {"verbose", no_argument,       &verbose_flag , 1},
               {"brief",   no_argument,       &verbose_flag, 0},
               /* These options don't set a flag.
                  We distinguish them by their indices. */
               {"listtasks",   no_argument,       0, 'l'},
               {"listconditions",   no_argument,       0, 'C'},
               {"check",    no_argument,       0, 'c'},
               {"listdocuments",    no_argument,       0, 'd'},
               {"data",    no_argument,       0, 'D'},
               {"file",  required_argument, 0, 'f'},
               {"autofilter",  required_argument, 0, 'a'},
               {"recursivefilter",  required_argument, 0, 'r'},
               {"task",  required_argument, 0, 't'},
                {"template",  no_argument, 0, 'T'},
               {"variable",  required_argument, 0, 'v'},
               {"json",  required_argument, 0, 'j'},
               {"graph",  required_argument, 0, 'g'},
               {"sign",  no_argument, 0, 's'},
               {"keyvalue",  required_argument, 0, 'k'},
               {"plugins",  required_argument, 0, 'p'},
               {"stat",  required_argument, 0, 'S'},
               {"help",  no_argument, 0, 'h'},
               {"version",  no_argument, 0, 'V'},
               {0, 0, 0, 0}
             };
           /* getopt_long stores the option index here. */
           int option_index = 0;

           c = getopt_long (argc, argv, "lCcf:a:t:Tj:dv:g:k:sS:hVD:p:",
                            long_options, &option_index);

           if (c == -1)
             break;
//           qDebug("optarg: %s", optarg);
           switch (c)
             {
             case 0:
               /* If this option set a flag, do nothing else now. */
               if (long_options[option_index].flag != 0)
                 break;
               streamText << tr("Opcion: ") << long_options[option_index].name ;
               if (optarg)
                 streamText << tr(" con argumento: ") <<  optarg << endl;
               break;
             case 'l':
                        commands['l'] = "list";
               break;
             case 'C':
                        commands['C'] = "list";
               break;
             case 'd':
                        commands['d'] = "documents";
               break;
             case 'D':
                        commands['D'] = optarg;
               break;
             case 'c':
                        commands['c'] = "check";
                       break;
             case 'p':
                        commands['p'] = optarg;
                       break;
              case 'f':
                        commands['f'] = optarg;
                       break;
              case 'a':
                       if (!commands.contains('a') ) {
                            commands['a'] = optarg;
                       }
                       else {
                            commands['a'] = commands['a']+"," +QString(optarg);
                       }
                       break;
              case 'r':
                       if (!commands.contains('r') ) {
                            commands['r'] = optarg;
                       }
                       else {
                            commands['r'] = commands['r']+"," +QString(optarg);
                       }
                       break;
              case 'g':
                        commands['g'] = optarg;
                       break;
             case 'v':
                        commands['v'] = optarg;
                        //(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!commands['v'].trimmed().startsWith("-"), tr("Debe especificar un nombre de variable, falta este parametro")) );
                        if (commands['v'].trimmed().startsWith("-") ) {
                            SafetYAWL::streamlog
                                    << SafetLog::Error
                                    <<
                            tr("Debe especificar un nombre de variable, falta este parametro");
                        }
                       break;
             case 'k':
                        commands['k'] = optarg;
//			(*SafetYAWL::evalExit())( SafetYAWL::streamlog.eval(!commands['k'].trimmed().startsWith("-"), tr("Debe especificar un valor para la clave, falta este parametro")) );
                        break;
             case 't':
                        commands['t'] = optarg;
                        break;
             case 'T':
                        commands['T'] = "template";
                        break;
             case 's':
                        commands['s'] = "sign";
                        break;
             case 'S':
                        commands['S'] = optarg;
                        break;
             case 'h':

                        commands['h'] = "help";
                        break;
             case 'V':
                        commands['V'] = "version";
             case '?':
               /* getopt_long already printed an error message. */
               break;

             default:
                        parsed = false;
             }
         }

       //	qDebug("!commands.contains('f'): %d", !commands.contains('f'));
        if ( commands.contains('h') || commands.contains('V') || commands.contains('T')) return true;
        if ( !commands.contains('f')  ) {
                streamText << tr("*** No se especifico la ruta del archivo de flujo de trabajo (.xml) *** \n");
                streamText  <<  tr("Opcion: -f <archivo> o --file <archivo> \n");
//		streamText.flush();
                sendStreamToOutput();
                parsed = false;
                processCommand('f');
        }

       if (verbose_flag)
         streamText << tr("La opción (verbose) está activa\n");

       /* Print any remaining command line arguments (not options). */
       if (optind < argc)
         {
//	   qDebug("optind: %d argc: %d", optind, argc);
           streamText << tr("Parametro(s) invalido(s): ");
           while (optind < argc)
             streamText <<  argv[optind++];
             streamText << endl;
                parsed = false;
         }

        if (!parsed ) return false;

        configurator->openXML(commands['f']);
  //      configurator->openDataSources();
        configurator->convertXMLtoObjects();

        // llamada a la funcion que habilita las fuentes de datos
        //qDebug("MainWindow: configurator->openDataSources()");

        //qDebug("SafetYAWL::databaseOpen: %d", SafetYAWL::databaseOpen);

        //configurator->loadPlugins();

        if ( commands.contains('c') ) {
                streamText << tr("Chequeado...!") << endl;
        }

        return parsed;
}

