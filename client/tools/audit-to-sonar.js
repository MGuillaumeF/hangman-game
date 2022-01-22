(async function() {
  exitStatus = 0;
  // get arguments of process run
  const args = process.argv.slice(2);
  // if args have good size run converting
  if (args.length === 1) {
      const [inputFile] = args;
      const fs = require('fs').promises;
      const path = require('path');
      const filename = path.resolve(process.cwd(), inputFile);
      let auditJsonString = '';
      try {
          auditJsonString = await fs.readFile(filename);
          auditJsonString = auditJsonString.toString();

      } catch (error) {
          console.error('input file read failed', error);
          exitStatus = 3;
          process.exit(3);
      }

      const audit = JSON.parse(auditJsonString);
      const issues = [];
      for (const [packageName, vulnerability] of Object.entries(audit.vulnerabilies)) {
          console.info('severity found', vulnerability.severity);
          issues.push({
              engineId : 'npm-audit',
              ruleId : 'dependency-vulnerability',
              severity : 'INFO',
              type : 'VULNERABILITY',
              primaryLocation : {
                  message : `The dependency ${packageName} has vulnerability`,
                  filePath : path.resolve(process.cwd(), 'package.json'),
                  textRange : {
                      startLine : 1,
                      startColumn : 0
                  }
              }
          });
      }
         
      const output = JSON.stringify({issues}, null, 4);
      console.debug('issues generated', output);
      try {
          await fs.writeFile(path.resolve(process.cwd(), 'dist/reports/audit-report.json'), output);
      } catch (error) {
          console.error('output file write failed', error);
          exitStatus = 2;
      }
  } else {
      console.error('one argument expected,', args.length, 'found');
      exitStatus = 1;
  }
  process.exit(exitStatus);
})();
