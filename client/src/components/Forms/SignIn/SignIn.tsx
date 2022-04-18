import React from "react";
import { useTranslation } from "react-i18next";
import api2 from "../../../generated/.api_doc_parameters.json";
import BasicInput from "../../BasicInput/BasicInput";
import BasicForm from "../BasicForm";
import { HttpMethod } from "../FormTypes";

const FIELDS = {
  login: {
    ...api2.userSignIn.post.login,
    id: "FORMS_SIGN_IN_FIELDS_IDENTIFIER"
  },
  password: {
    ...api2.userSignIn.post.password,
    id: "FORMS_SIGN_IN_FIELDS_PASSWORD"
  }
};

type Props = {
  id: string;
};

const loginInputProperties = { required: FIELDS.login.required };
const passwordInputProperties = { required: FIELDS.password.required };

function SignIn({ id }: Props) {
  const { t } = useTranslation();

  return (
    <>
      <BasicForm
        method={HttpMethod.POST}
        action={`/api${api2.userSignIn.path}`}
        fields={Object.values(FIELDS)}
        submitTitle="FORMS.SIGN_IN.FIELDS.SUBMIT.LABEL"
        id={id}
      >
        <fieldset>
          <legend>{String(t("FORMS.SIGN_IN.TITLE"))}</legend>
          <BasicInput
            type="text"
            name={FIELDS.login.name}
            id={FIELDS.login.id}
            inputProperties={loginInputProperties}
            label={t("FORMS.SIGN_IN.FIELDS.IDENTIFIER.LABEL")}
          />

          <BasicInput
            type="password"
            name={FIELDS.password.name}
            id={FIELDS.password.id}
            inputProperties={passwordInputProperties}
            label={t("FORMS.SIGN_IN.FIELDS.PASSWORD.LABEL")}
          />
        </fieldset>
      </BasicForm>
    </>
  );
}

export default SignIn;
