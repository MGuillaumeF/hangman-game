import { load } from "loader";

export generateTsClasses(modelClasses) {
  console.log(load("TsClasses", {
  dependencies : "",
  className: "",
  extendedClasses : "",
  privateAttributes : "",
  privateStaticAttributes:"",
  privateMethods:"",
  privateStaticMethods : "",
  protectedAttributes : "",
  protectedStaticAttributes : "",
  protectedMethods : "",
  protectedStaticMethods : "",
  publicAttributes : "",
  publicStaticAttributes : "",
  publicMethods : "",
  publicStaticMethods : ""
});
}
